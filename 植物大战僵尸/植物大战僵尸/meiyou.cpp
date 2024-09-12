using O2DESNet; // ����O2DESNet�⣬���ڹ�����������ɢ�¼�����ģ�͡�
using WSC_SimChallenge_2024_Net.Activity; // �����Զ��������ռ��е�Activityģ�飬����������ص��ࡣ

namespace WSC_SimChallenge_2024_Net.PortSimulation.Entity // ���������ռ䣬��֯�͸�����ۿڷ���ʵ����صĴ��롣
{
    public class Vessel // ����һ����ΪVessel�Ĺ����࣬��ʾ�ۿڷ����еĴ���ʵ�塣
    {
        public int Week; // ��ʾ�����������������������ڿ��Ʒ����е�ʱ��Ρ�
        public string Id; // ������Ψһ��ʶ����
        public int AlreadyAllocatedQcCount = 0; // �ѷ���İ�����������ʼֵΪ0��
        public int RequiredQcCount = 3; // ����İ���������Ĭ��Ϊ3��
        public Berth AllocatedBerth = null; // �������䵽�Ĳ�λ����ʼֵΪnull����ʾ��δ���䡣
        public List<QC> AllocatedQCs = new List<QC>(); // �ѷ���İ����б����ڸ�����ô��������İ��š�
        public QCLine UsedQCLine = null; // ʹ�õ�QC�ߣ����Ų��������̣�����ʼֵΪnull��
        public Dictionary<string, int> DischargingContainersInformation = new Dictionary<string, int>(); // �洢ж����װ�����Ϣ����ΪĿ�ĵأ�ֵΪ��װ��������
        public Dictionary<string, int> LoadingContainersInformation = new Dictionary<string, int>(); // �洢װ����װ�����Ϣ����Ϊ��Դ�أ�ֵΪ��װ��������
        public DateTime ArrivalTime; // ��������ۿڵ�ʱ�䡣
        public DateTime StartBerthingTime; // ������ʼͣ����ʱ�䡣

        public override string ToString() // ��дToString���������崬��������ַ�����ʾ��ʽ��
        {
            return $"Vessel[{Id}]"; // ���ظ�ʽ���ַ�������ʾ������Id����"Vessel[123]"�����ڵ��Ի���־��¼��
        }

        // ����һ��Ƕ����Waiting���̳���BaseActivity<Vessel>����ʾ�����ڵȴ�״̬ʱ�Ļ��
        public class Waiting : BaseActivity<Vessel>
        {
            // ���캯������ʼ��Waiting��ࡣ
            public Waiting(bool debugMode = false, int seed = 0) : base(nameof(Waiting), debugMode, seed)
            {
                _debugMode = PortSimModel.DebugofVessel; // ���õ���ģʽ������PortSimModel�ĵ��Կ��ء�
                TimeSpan = TimeSpan.Zero; // ���û��ʱ����Ϊ�㣬��ʾ˲ʱ�¼���
            }

            // ��ʼ��ķ������������ĵ��
            public override void Start(Vessel vessel)
            {
                if (_debugMode) Console.WriteLine($"{ClockTime.ToString("yyyy - MM - dd HH : mm:ss")}  {ActivityName}.Start({vessel})");
                vessel.ArrivalTime = ClockTime; // ���ô����ĵ���ʱ��Ϊ��ǰ����ʱ�䡣
                HourCounter.ObserveChange(1); // ��¼ʱ��䶯��
                ProcessingList.Add(vessel); // ��������ӵ����ڴ�����б��С�
                PendingList.Remove(vessel); // �Ӵ������б����Ƴ�������
                ReadyToStartList.Remove(vessel); // ��׼����ʼ���б����Ƴ�������
                Schedule(() = > Complete(vessel), TimeSpan); // ����һ����ɸû���¼���
                EmitOnStart(vessel); // �������ʼ�¼���
            }
        }

        // ����һ��Ƕ����Berthing���̳���BaseActivity<Vessel>����ʾ������ͣ��״̬ʱ�Ļ��
        public class Berthing : BaseActivity<Vessel>
        {
            // ���캯������ʼ��Berthing��ࡣ
            public Berthing(bool debugMode = false, int seed = 0) : base(nameof(Berthing), debugMode, seed)
            {
                _debugMode = PortSimModel.DebugofVessel; // ���õ���ģʽ������PortSimModel�ĵ��Կ��ء�
                NeedExtTryStart = true; // ��ʾ�Ƿ���Ҫ�ⲿ�������Կ�ʼ���
                NeedExtTryFinish = true; // ��ʾ�Ƿ���Ҫ�ⲿ����������ɻ��
                TimeSpan = TimeSpan.Zero; // ���û��ʱ����Ϊ�㣬��ʾ˲ʱ�¼���
            }

            // ����ʼ��ķ�������������ӵ��������б��У������ȳ��Կ�ʼ���
            public override void RequestToStart(Vessel vessel)
            {
                PendingList.Add(vessel); // ��������ӵ��������б��С�
                Schedule(() = > AttemptToStart(), TimeSpan.FromMicroseconds(1)); // ���ȳ��Կ�ʼ���ʱ����Ϊ1΢�롣
            }

            // ���Կ�ʼ��ķ��������ݴ����Ĳ�λ�Ͱ��ŷ�������������Ƿ�ʼ��
            public override void TryStart(Object obj)
            {
                if (_debugMode) Console.WriteLine($"{ClockTime.ToString("yyyy - MM - dd HH : mm:ss")}  {ActivityName}.TryStart({obj})");

                Berth ? berth = (obj is Berth) ? (obj as Berth) : null; // �жϴ�������Ƿ�Ϊ��λ������������ת����
                QC ? qc = (obj is QC) ? (obj as QC) : null; // �жϴ�������Ƿ�Ϊ���ţ�����������ת����

                if (berth == null & qc == null) { return; } // ����Ȳ��ǲ�λҲ���ǰ��ţ���ֱ�ӷ��ء�

                if (berth != null) // ����������Դ�ǲ�λ
                {
                    Vessel vessel = PendingList.Find(v = > v.AllocatedBerth == berth); // ���ҷ����˸ò�λ�Ĵ�����

                    if (vessel.AllocatedQCs.Count == vessel.RequiredQcCount && vessel.AllocatedBerth != null)
                    {
                        ReadyToStartList.Add(vessel); // �����������İ�����������Ҫ�����Ѿ������˲�λ����������ӵ�׼����ʼ�б��С�
                        AttemptToStart(); // ���Կ�ʼ���
                    }
                }
                else if (qc != null) // ����������Դ�ǰ���
                {
                    Vessel vessel = PendingList.Find(v = > v == qc.ServedVessel); // ��������ʹ�øð��ŵĴ�����
                    vessel.AllocatedQCs.Add(qc); // ��������ӵ������ķ����б��С�

                    if (vessel.AllocatedQCs.Count == vessel.RequiredQcCount && vessel.AllocatedBerth != null)
                    {
                        ReadyToStartList.Add(vessel); // �������İ�����������Ҫ�����Ѿ������˲�λ����������ӵ�׼����ʼ�б��С�
                        AttemptToStart(); // ���Կ�ʼ���
                    }
                }
            }

            // ��ʼ��ķ�������������ͣ����
            public override void Start(Vessel vessel)
            {
                if (_debugMode) Console.WriteLine($"{ClockTime.ToString("yyyy - MM - dd HH : mm:ss")}  {ActivityName}.Start({vessel})");
                vessel.StartBerthingTime = ClockTime; // ���ô����Ŀ�ʼͣ��ʱ��Ϊ��ǰ����ʱ�䡣
                HourCounter.ObserveChange(1); // ��¼ʱ��䶯��
                ProcessingList.Add(vessel); // ��������ӵ����ڴ�����б��С�
                PendingList.Remove(vessel); // �Ӵ������б����Ƴ�������
                ReadyToStartList.Remove(vessel); // ��׼����ʼ���б����Ƴ�������
                Schedule(() = > Complete(vessel), TimeSpan); // ����һ����ɸû���¼���
                EmitOnStart(vessel); // �������ʼ�¼���
            }

            // ������ɻ�ķ���������Ƿ��������������
            public override void TryFinish(Object obj)
            {
                if (_debugMode) Console.WriteLine($"{ClockTime.ToString("yyyy - MM - dd HH : mm:ss")}  {ActivityName}.TryFinish({obj})");

                QCLine ? qcLine = (obj is QCLine) ? (obj as QCLine) : null; // �жϴ�������Ƿ�ΪQC�ߣ�����������ת����
                if (qcLine == null) { return; } // ������������QC�ߣ���ֱ�ӷ��ء�

                foreach(var vessel in CompletedList) // ����������б�����ʹ�ø�QC�ߵĴ�����
                {
                    if (qcLine.ServedVessel == vessel)
                    {
                        ReadyToFinishList.Add(vessel); // ����ҵ���ʹ�ø�QC�ߵĴ�����������ӵ�׼������б��С�
                        AttemptToFinish(vessel); // ������ɻ��
                        break; // ����ѭ������Ϊ�Ѿ��ҵ���Ŀ�괬����
                    }
                }
            }
        }
    }
}
