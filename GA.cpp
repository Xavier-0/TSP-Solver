#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

//�������� N
#define N 48
//��ʶ�����Ƿ񱻷���,���ʹ���Ϊ 1
bool visit[N];
//���о������
int distance[N][N];

//��Ⱥ����
#define POP_NUM 100
//��������
#define GENERATE_COUNT 2000
#define GEN 10
//�������
#define PC1 0.6
#define PC2 0.8
//�������
#define PM 0.2
//��Ⱥ
int population[POP_NUM][N];
//�����ľ�Ӣ����
int currentOpt[N];
int currentOptDistance;
int minIndex;
int maxIndex;
//��Ⱥƽ��������ܾ���
int avgDistance[POP_NUM];
int popDistance[POP_NUM];
int selectPop[POP_NUM][N];

void init()
{
	//���е� x �� y ����
	int x[N] = { 0 };
	int y[N] = { 0 };
	//�� data.txt �ļ���ȡ����
	FILE* fp;
	if ((fp = fopen("..//att48.txt", "r")) == NULL)
	//if ((fp = fopen("..//kroB100.txt", "r")) == NULL)
	{
		printf("can not open the file!");
		exit(0);
	}
	while (!feof(fp))
	{
		int count;
		fscanf(fp, "%d", &count);
		fscanf(fp, "%d%d", &x[count - 1], &y[count - 1]);
	}
	fclose(fp);
	//�������֮�����
	for (int i = 0; i < N - 1; i++)
	{
		distance[i][i] = 0;				// �Խ���Ϊ0
		for (int j = i + 1; j < N; j++)
		{
			double dis = sqrt((pow((double)x[i] - x[j], 2) / 10 + pow((double)y[i] - y[j], 2) / 10 ));
			int disInt = (int)dis;
			distance[i][j] = dis == disInt ? disInt : disInt + 1;
			distance[j][i] = distance[i][j];
		}
	}
	distance[N - 1][N - 1] = 0;
}

/* ̰�Ĳ��� */
void TSPGreedyAlgorithm()
{
	int totalDistance = 0;		//��·��
	//Ĭ�ϴ� 0 ��ʼ����
	int current = 0;			//��ǰѡȡ���	
	visit[0] = 1;
	printf("TSP ·��Ϊ��%d ->", 1);

	//���� N - 1 ��
	for (int i = 1; i < N; i++)
	{
		//���ýϴ�ľ����ʼֵ����ѡȡ�����
		int min_distance = 0x7fffffff;
		//���浱ǰ����ڳ���
		int temp;
		//ѭ��ѡȡ����
		for (int j = 1; j < N; j++)
		{
			if (!visit[j] && distance[current][j] < min_distance)
			{
				min_distance = distance[current][j];
				temp = j;
			}
		}
		visit[temp] = 1;
		current = temp;
		currentOpt[i] = temp;
		population[0][i] = temp;
		totalDistance += min_distance;
		printf(" %d ->", temp + 1);
	}
	totalDistance += distance[current][0];
	currentOptDistance = totalDistance;
	printf(" %d\n", 1);
}

/* ��ӡ��Ⱥ�����ԣ� */
void printTest()
{
	for (int i = 0; i < POP_NUM; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%d ", population[i][j]);
		}
		printf("\n");
	}
}

/* ������� num �θ�����г�ʼ�� */
void swap(int* array, int num)
{
	int first, last, temp;
	for (int i = 0; i < num; i++)
	{
		first = rand() % N;
		last = rand() % N;
		temp = array[first];
		array[first] = array[last];
		array[last] = temp;
	}
}

/* 2-opt ���� */
void reverse(int* array, int num)
{
	int first, last, temp;
	for (int i = 0; i < num; i++)
	{
		first = rand() % N;
		last = rand() % N;
		if (first > last)
		{
			temp = last;
			last = first;
			first = temp;
		}
		int sumBefore = 0;
		int sumAfter = 0;
		int sum = 0;
		for (int j = first; j < last; j++)
		{
			sum += distance[array[j]][array[j + 1]];
		}
		sumBefore = sum + distance[array[(first-1)%N]][array[first]] + distance[array[last]][array[(last + 1) % N]];
		sumAfter  = sum + distance[array[(first-1)%N]][array[last]] + distance[array[first]][array[(last + 1) % N]];
		if (sumAfter <= sumBefore)
		{
			for (int j = first; j < (last + first) / 2; j++)
			{
				temp = array[j];
				array[j] = array[first + last - j];
				array[first + last - j] = temp;
			}
		}
	}
}

/* ������Ⱥ�����ܾ���,����������Ⱥ�ܾ��� */
int calDistance()
{
	int minDistance = 0x7fffffff;
	int maxDistance = 0;
	int sum = 0;
	for (int i = 0; i < POP_NUM; i++)
	{
		popDistance[i] = 0;
		for (int j = 0; j < N - 1; j++)
		{
			popDistance[i] += distance[population[i][j]][population[i][j + 1]];
		}
		popDistance[i] += distance[population[i][N - 1]][population[i][0]];
		sum += popDistance[i];
		//���������̵ĸ���·������
		if (popDistance[i] < minDistance)
		{
			minIndex = i;
			minDistance = popDistance[i];
		}
		if (popDistance[i] > maxDistance)
		{
			maxIndex = i;
			maxDistance = popDistance[i];
		}
	}
	//�������Ÿ���
	if (popDistance[minIndex] < currentOptDistance)
	{
		currentOptDistance = popDistance[minIndex];
		for (int i = 0; i < N; i++)
		{
			currentOpt[i] = population[minIndex][i];
		}
	}
	//��̭���ĸ���
	for (int i = 0; i < N; i++)
	{
		population[maxIndex][i] = currentOpt[i];
	}
	printf("��ǰ�������Ϊ��%d\n", popDistance[minIndex]);
	return sum;
	//printf("��ǰ��Զ����Ϊ��%d\n", popDistance[maxIndex]);
}

/* ��ʼ����Ⱥ */
void initPop()
{
	//��������õ�Ŀ��������Ⱥ
	for (int i = 1; i < POP_NUM; i++)
	{
		for (int j = 0; j < N; j++)
		{
			population[i][j] = population[0][(j + i) % N];
		}
		swap(population[i], 1);
	}
	calDistance();
}

/* ѡ����Ӧ�ȴ�����屻�Ŵ�����һ��Ⱥ���еĸ���
   ���̶�ѡ�񷽷���ʵ�ֲ���:
	��1������Ⱥ�������и������Ӧ��ֵ��
	��2������ÿ�������ѡ����ʣ�
	��3��������۸��ʣ�
	��4������ģ����̲�����������0��1֮����������ÿ�������Ŵ�����һ��Ⱥ��ĸ��ʽ���ƥ�䣩��ȷ�����������Ƿ��Ŵ�����һ��Ⱥ���С�
*/
void select()
{
	double popFit[POP_NUM];				//��Ⱥ������Ӧ��
	double p[POP_NUM];					//��Ⱥ�����ѡ�����
	double sum = 0;
	//��Ӧ��
	for (int i = 0; i < POP_NUM; i++)
	{
		popFit[i] = 10000.0 / popDistance[i];	//��Ӧ�Ⱥ���֮Ϊ����ĵ�����ע�����Ϊ double �����Ϊ double
		sum += popFit[i];
	}
	//�ۼƸ���
	for (int i = 0; i < POP_NUM; i++)
	{
		p[i] = popFit[i] / sum;
	}
	//������Ӣ����
	for (int k = 0; k < N; k++)
	{
		selectPop[0][k] = population[minIndex][k];
	}
	//���̶�ѡ��
	for (int i = 1; i < POP_NUM; i++)
	{
		double temp = ((double)rand()) / RAND_MAX;
		for (int j = 0; j < POP_NUM; j++)
		{
			temp -= p[j];
			if (temp <= 0)
			{
				for (int k = 0; k < N; k++)
				{
					selectPop[i][k] = population[j][k];
				}
				break;
			}
		}
	}
}

/* ���棺�����໥��Ե�Ⱦɫ�����ݽ������ PC ��ĳ�ַ�ʽ�໥�����䲿�ֻ����γ������µĸ��� 
 * cross1():��1�����ѡ��һ�������Ļ����������������ҵ��û����λ�ã���Ϊ�Ӵ���һ������
 *			��2���ж���һ�������Ƿ����Ӵ��У���Ϊ 4 ��������Ƚϸ���������������ľ��룬��Ϊ�Ӵ��еĻ���
 *			��3���ظ���2�����裬ֱ���γɸ���
 *          ��4�����Ӵ���ת��Ϊ��һ���Ӵ�
 * �ý����������̰��˼�룬���Լ̳и�������Ļ���Ѹ��������
 *cross2()�����㽻�棬��չ��Ⱥ�ĸ��Ƿ�Χ
 */
void cross1()
{
	//pos1��pos2����ѡȡ�Ļ���
	int pos1;
	int pos2;
	for (int k = 0; k < POP_NUM - 1; k += 2)
	{
		if (((double)rand()) / RAND_MAX < PC1)		//�������
		{
			pos1 = 0;
			pos2 = 0;
			bool flag[N] = { 0 };
			//���ѡȡ��ʼ����
			population[k][0] = rand() % N;
			flag[population[k][0]] = 1;
			for (int i = 0; i < N - 1; i++)
			{
				//�ҵ���ͬ��˫�׽ڵ�
				for (int j = 0; j < N; j++)
				{
					if (population[k][i] == selectPop[k][j])
					{
						pos1 = j;
						break;
					}
				}
				for (int j = 0; j < N; j++)
				{
					if (population[k][i] == selectPop[k + 1][j])
					{
						pos2 = j;
						break;
					}
				}
				//printf("%d %d, %d, %d\n", pos1, pos2, population[k][i], i);
				bool add = false;
				while (!add)
				{
					if (!flag[selectPop[k + 1][(pos2 + 1) % N]] && !flag[selectPop[k][(pos1 + 1) % N]] && distance[population[k][i]][selectPop[k][(pos1 + 1) % N]] <= distance[population[k][i]][selectPop[k + 1][(pos2 + 1) % N]] || !flag[selectPop[k][(pos1 + 1) % N]] && flag[selectPop[k + 1][(pos2 + 1) % N]])
					{
						population[k][i + 1] = selectPop[k][(pos1 + 1) % N];
						flag[population[k][i + 1]] = 1;
						add = true;
					}
					else if (!flag[selectPop[k + 1][(pos2 + 1) % N]] && !flag[selectPop[k][(pos1 + 1) % N]] && distance[population[k][i]][selectPop[k][(pos1 + 1) % N]] > distance[population[k][i]][selectPop[k + 1][(pos2 + 1) % N]] || flag[selectPop[k][(pos1 + 1) % N]] && !flag[selectPop[k + 1][(pos2 + 1) % N]])
					{
						population[k][i + 1] = selectPop[k + 1][(pos2 + 1) % N];
						flag[population[k][i + 1]] = 1;
						add = true;
					}
					else if (!add && flag[selectPop[k][(pos1 + 1) % N]] && flag[selectPop[k + 1][(pos2 + 1) % N]])
					{
						pos1 = (pos1 + 1) % N;
						pos2 = (pos2 + 1) % N;
					}
				}
			}
			for (int i = 0; i < N; i++)
			{
				population[k + 1][i] = population[k][N - 1 - i];
			}
		}	
	}
}

void cross2()
{
	//�����λ��
	int ranPos1;
	int ranPos2;
	int temp;
	for (int k = 0; k < POP_NUM - 1; k += 2)
	{
		ranPos1 = rand() % N;
		ranPos2 = rand() % N;
		if (((double)rand()) / RAND_MAX < PC2)		//�������
		{
			if (ranPos1 > ranPos2)
			{
				temp = ranPos1;
				ranPos1 = ranPos2;
				ranPos2 = temp;
			}
			for (int i = ranPos1; i <= ranPos2; i++)
			{
				temp = population[k][i];
				population[k][i] = population[k + 1][i];
				population[k + 1][i] = temp;
			}
			int count1 = 0;
			int count2 = 0;
			int flag1[N];
			int flag2[N];
			for (int i = 0; i <= ranPos1 - 1; i++)
			{
				for (int j = ranPos1; j <= ranPos2; j++)
				{
					if (population[k][i] == population[k][j])
					{
						flag1[count1] = i;
						count1++;
					}
					if (population[k + 1][i] == population[k + 1][j])
					{
						flag2[count2] = i;
						count2++;
					}
				}
			}
			for (int i = ranPos2 + 1; i < N; i++)
			{
				for (int j = ranPos1; j <= ranPos2; j++)
				{
					if (population[k][i] == population[k][j])
					{
						flag1[count1] = i;
						count1++;
					}
					if (population[k + 1][i] == population[k + 1][j])
					{
						flag2[count2] = i;
						count2++;
					}
				}
			}
			if (count1 == count2 && count1 > 0)
			{
				for (int i = 0; i < count1; i++)
				{
					temp = population[k][flag1[i]];
					population[k][flag1[i]] = population[k + 1][flag2[i]];
					population[k + 1][flag2[i]] = temp;
				}
			}
		}
	}
}

/* ���죺�ı������봮�е�ĳЩ����ֵ���Ӷ��γ��µĸ��� */
void mutate(int param)
{
	for (int k = 0; k < POP_NUM; k++)
	{
		if (((double)rand()) / RAND_MAX < PM)	//�������
		{
			reverse(population[k], param);
		}
	}
}


int main()
{
	//��ʼ��
	init();
	//��ǰʱ����Ϊ���������
	srand((unsigned)time(NULL));
	//̰���㷨��� TSP 
	TSPGreedyAlgorithm();
	
	initPop();
	for (int i = 0; i < GENERATE_COUNT; i++)
	{
		//���㵱ǰ��Ⱥƽ������	
		select();
		if (i >= GEN && abs(avgDistance[i] - avgDistance[i - GEN]) < 10 && popDistance[minIndex] == currentOptDistance)
		{
			cross2();
			mutate(N);
		}
		else
		{
			cross1();
			mutate(2);
		}
		//������Ⱥƽ��·�����Ȳ��������·��
		avgDistance[i] = calDistance() / POP_NUM;
	}
	printf("\n���·������Ϊ��%d\n", currentOptDistance);
	printf("TSP ·��Ϊ��");
	for (int i = 0; i < N; i++)
	{
		printf("%d -> ", currentOpt[i]);
	}
	printf("%d", currentOpt[0]);
	return 0;
}