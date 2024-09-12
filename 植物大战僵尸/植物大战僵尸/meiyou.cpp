using O2DESNet; // 引用O2DESNet库，用于构建和运行离散事件仿真模型。
using WSC_SimChallenge_2024_Net.Activity; // 引用自定义命名空间中的Activity模块，包含仿真活动相关的类。

namespace WSC_SimChallenge_2024_Net.PortSimulation.Entity // 定义命名空间，组织和隔离与港口仿真实体相关的代码。
{
    public class Vessel // 定义一个名为Vessel的公共类，表示港口仿真中的船舶实体。
    {
        public int Week; // 表示船舶所属的周数，可能用于控制仿真中的时间段。
        public string Id; // 船舶的唯一标识符。
        public int AlreadyAllocatedQcCount = 0; // 已分配的岸桥数量，初始值为0。
        public int RequiredQcCount = 3; // 所需的岸桥数量，默认为3。
        public Berth AllocatedBerth = null; // 船舶分配到的泊位，初始值为null，表示尚未分配。
        public List<QC> AllocatedQCs = new List<QC>(); // 已分配的岸桥列表，用于跟踪与该船舶关联的岸桥。
        public QCLine UsedQCLine = null; // 使用的QC线（岸桥操作的流程），初始值为null。
        public Dictionary<string, int> DischargingContainersInformation = new Dictionary<string, int>(); // 存储卸货集装箱的信息，键为目的地，值为集装箱数量。
        public Dictionary<string, int> LoadingContainersInformation = new Dictionary<string, int>(); // 存储装货集装箱的信息，键为来源地，值为集装箱数量。
        public DateTime ArrivalTime; // 船舶到达港口的时间。
        public DateTime StartBerthingTime; // 船舶开始停泊的时间。

        public override string ToString() // 重写ToString方法，定义船舶对象的字符串表示形式。
        {
            return $"Vessel[{Id}]"; // 返回格式化字符串，显示船舶的Id，如"Vessel[123]"，用于调试或日志记录。
        }

        // 定义一个嵌套类Waiting，继承自BaseActivity<Vessel>，表示船舶在等待状态时的活动。
        public class Waiting : BaseActivity<Vessel>
        {
            // 构造函数，初始化Waiting活动类。
            public Waiting(bool debugMode = false, int seed = 0) : base(nameof(Waiting), debugMode, seed)
            {
                _debugMode = PortSimModel.DebugofVessel; // 设置调试模式，根据PortSimModel的调试开关。
                TimeSpan = TimeSpan.Zero; // 设置活动的时间跨度为零，表示瞬时事件。
            }

            // 开始活动的方法，处理船舶的到达。
            public override void Start(Vessel vessel)
            {
                if (_debugMode) Console.WriteLine($"{ClockTime.ToString("yyyy - MM - dd HH : mm:ss")}  {ActivityName}.Start({vessel})");
                vessel.ArrivalTime = ClockTime; // 设置船舶的到达时间为当前仿真时间。
                HourCounter.ObserveChange(1); // 记录时间变动。
                ProcessingList.Add(vessel); // 将船舶添加到正在处理的列表中。
                PendingList.Remove(vessel); // 从待处理列表中移除船舶。
                ReadyToStartList.Remove(vessel); // 从准备开始的列表中移除船舶。
                Schedule(() = > Complete(vessel), TimeSpan); // 调度一个完成该活动的事件。
                EmitOnStart(vessel); // 触发活动开始事件。
            }
        }

        // 定义一个嵌套类Berthing，继承自BaseActivity<Vessel>，表示船舶在停泊状态时的活动。
        public class Berthing : BaseActivity<Vessel>
        {
            // 构造函数，初始化Berthing活动类。
            public Berthing(bool debugMode = false, int seed = 0) : base(nameof(Berthing), debugMode, seed)
            {
                _debugMode = PortSimModel.DebugofVessel; // 设置调试模式，根据PortSimModel的调试开关。
                NeedExtTryStart = true; // 表示是否需要外部条件尝试开始活动。
                NeedExtTryFinish = true; // 表示是否需要外部条件尝试完成活动。
                TimeSpan = TimeSpan.Zero; // 设置活动的时间跨度为零，表示瞬时事件。
            }

            // 请求开始活动的方法，将船舶添加到待处理列表中，并调度尝试开始活动。
            public override void RequestToStart(Vessel vessel)
            {
                PendingList.Add(vessel); // 将船舶添加到待处理列表中。
                Schedule(() = > AttemptToStart(), TimeSpan.FromMicroseconds(1)); // 调度尝试开始活动，时间间隔为1微秒。
            }

            // 尝试开始活动的方法，根据船舶的泊位和岸桥分配情况来决定是否开始。
            public override void TryStart(Object obj)
            {
                if (_debugMode) Console.WriteLine($"{ClockTime.ToString("yyyy - MM - dd HH : mm:ss")}  {ActivityName}.TryStart({obj})");

                Berth ? berth = (obj is Berth) ? (obj as Berth) : null; // 判断传入对象是否为泊位，并进行类型转换。
                QC ? qc = (obj is QC) ? (obj as QC) : null; // 判断传入对象是否为岸桥，并进行类型转换。

                if (berth == null & qc == null) { return; } // 如果既不是泊位也不是岸桥，则直接返回。

                if (berth != null) // 如果传入的资源是泊位
                {
                    Vessel vessel = PendingList.Find(v = > v.AllocatedBerth == berth); // 查找分配了该泊位的船舶。

                    if (vessel.AllocatedQCs.Count == vessel.RequiredQcCount && vessel.AllocatedBerth != null)
                    {
                        ReadyToStartList.Add(vessel); // 如果船舶分配的岸桥数量满足要求并且已经分配了泊位，将船舶添加到准备开始列表中。
                        AttemptToStart(); // 尝试开始活动。
                    }
                }
                else if (qc != null) // 如果传入的资源是岸桥
                {
                    Vessel vessel = PendingList.Find(v = > v == qc.ServedVessel); // 查找正在使用该岸桥的船舶。
                    vessel.AllocatedQCs.Add(qc); // 将岸桥添加到船舶的分配列表中。

                    if (vessel.AllocatedQCs.Count == vessel.RequiredQcCount && vessel.AllocatedBerth != null)
                    {
                        ReadyToStartList.Add(vessel); // 如果分配的岸桥数量满足要求并且已经分配了泊位，将船舶添加到准备开始列表中。
                        AttemptToStart(); // 尝试开始活动。
                    }
                }
            }

            // 开始活动的方法，处理船舶的停泊。
            public override void Start(Vessel vessel)
            {
                if (_debugMode) Console.WriteLine($"{ClockTime.ToString("yyyy - MM - dd HH : mm:ss")}  {ActivityName}.Start({vessel})");
                vessel.StartBerthingTime = ClockTime; // 设置船舶的开始停泊时间为当前仿真时间。
                HourCounter.ObserveChange(1); // 记录时间变动。
                ProcessingList.Add(vessel); // 将船舶添加到正在处理的列表中。
                PendingList.Remove(vessel); // 从待处理列表中移除船舶。
                ReadyToStartList.Remove(vessel); // 从准备开始的列表中移除船舶。
                Schedule(() = > Complete(vessel), TimeSpan); // 调度一个完成该活动的事件。
                EmitOnStart(vessel); // 触发活动开始事件。
            }

            // 尝试完成活动的方法，检查是否满足完成条件。
            public override void TryFinish(Object obj)
            {
                if (_debugMode) Console.WriteLine($"{ClockTime.ToString("yyyy - MM - dd HH : mm:ss")}  {ActivityName}.TryFinish({obj})");

                QCLine ? qcLine = (obj is QCLine) ? (obj as QCLine) : null; // 判断传入对象是否为QC线，并进行类型转换。
                if (qcLine == null) { return; } // 如果传入对象不是QC线，则直接返回。

                foreach(var vessel in CompletedList) // 遍历已完成列表，查找使用该QC线的船舶。
                {
                    if (qcLine.ServedVessel == vessel)
                    {
                        ReadyToFinishList.Add(vessel); // 如果找到了使用该QC线的船舶，将其添加到准备完成列表中。
                        AttemptToFinish(vessel); // 尝试完成活动。
                        break; // 结束循环，因为已经找到了目标船舶。
                    }
                }
            }
        }
    }
}
