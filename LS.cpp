////////////////////////
//TSP���� �����ֲ�����������
//����Berlin52�������
//���ߣ�infinitor
//ʱ�䣺2018-04-12
////////////////////////


#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <windows.h>
#include <memory.h>
#include <string.h>
#include <iomanip>

#define DEBUG

using namespace std;

#define CITY_SIZE 52 //��������


//��������
typedef struct candidate
{
    int x;
    int y;
}city, CITIES;

//�Ż�ֵ
int **Delta; 

//�������
typedef struct Solution
{
    int permutation[CITY_SIZE]; //��������
    int cost;                        //�����ж�Ӧ����·�߳���
}SOLUTION;
// ������������Ż�ֵ 
int calc_delta(int i, int k, int *tmp, CITIES * cities);

//�����������м����
int distance_2city(city c1, city c2);

//���ݲ����ĳ������У����������ܾ���
int cost_total(int * cities_permutation, CITIES * cities);

//��ȡ�����������, ���ڲ�����ʼ��
void random_permutation(int * cities_permutation);

//�ߵ��������±�begin��end��Ԫ��λ��, ����two_opt������
void swap_element(int *p, int begin, int end);

//������ ��תindex_i <-> index_j ���Ԫ��
void two_opt_swap(int *cities_permutation, int *new_cities_permutation, int index_i, int index_j);

//���ؾֲ��������߽����� max_no_improve
void local_search(SOLUTION & best, CITIES * cities, int max_no_improve);

//�жϽ���׼��
bool AcceptanceCriterion(int *cities_permutation, int *old_cities_permutation, CITIES * p_cities);

//���������зֳ�4�飬Ȼ�󰴿����´���˳��
//�����Ŷ�����
void double_bridge_move(int *cities_permutation, int * new_cities_permutation);

//�Ŷ�
void perturbation(CITIES * cities, SOLUTION &best_solution, SOLUTION &current_solution);

//��������
void iterated_local_search(SOLUTION & best, CITIES * cities, int max_iterations, int max_no_improve);

// ����Delta 
void Update(int i, int k,  int *tmp, CITIES * cities);

//��������
int permutation[CITY_SIZE];
//������������
CITIES cities[CITY_SIZE];


//berlin52�������꣬���Ž�7542����
CITIES berlin52[CITY_SIZE] = { { 565,575 },{ 25,185 },{ 345,750 },{ 945,685 },{ 845,655 },
{ 880,660 },{ 25,230 },{ 525,1000 },{ 580,1175 },{ 650,1130 },{ 1605,620 },
{ 1220,580 },{ 1465,200 },{ 1530,5 },{ 845,680 },{ 725,370 },{ 145,665 },
{ 415,635 },{ 510,875 },{ 560,365 },{ 300,465 },{ 520,585 },{ 480,415 },
{ 835,625 },{ 975,580 },{ 1215,245 },{ 1320,315 },{ 1250,400 },{ 660,180 },
{ 410,250 },{ 420,555 },{ 575,665 },{ 1150,1160 },{ 700,580 },{ 685,595 },
{ 685,610 },{ 770,610 },{ 795,645 },{ 720,635 },{ 760,650 },{ 475,960 },
{ 95,260 },{ 875,920 },{ 700,500 },{ 555,815 },{ 830,485 },{ 1170,65 },
{ 830,610 },{ 605,625 },{ 595,360 },{ 1340,725 },{ 1740,245 } };

int main()
{
	srand(1);
    int max_iterations = 600;
    int max_no_improve = 50;
	//��ʼ��ָ������ 
	Delta = new int*[CITY_SIZE];
	for (int i = 0; i < CITY_SIZE; i ++)
		Delta[i] = new int[CITY_SIZE];
	
    SOLUTION best_solution;

    iterated_local_search(best_solution, berlin52, max_iterations, max_no_improve);

    cout << endl<<endl<<"������ɣ� ����·���ܳ��� = " << best_solution.cost << endl;
    cout << "���ŷ��ʳ����������£�" << endl;
    for (int i = 0; i < CITY_SIZE;i++)
    {
        cout << setw(4) << setiosflags(ios::left) << best_solution.permutation[i];
    }

    cout << endl << endl;

    return 0;
}



//�����������м����
int distance_2city(city c1, city c2)
{
    int distance = 0;
    distance = sqrt((double)((c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y)));

    return distance;
}

//���ݲ����ĳ������У����������ܾ���
//��ν�������У����ǳ����Ⱥ���ʵ�˳�򣬱�������ȷ���ABC��Ҳ�����ȷ���BAC�ȵ�
//����˳��ͬ����ô��·�߳���Ҳ�ǲ�ͬ��
//p_perm �������в���
int cost_total(int * cities_permutation, CITIES * cities)
{
    int total_distance = 0;
    int c1, c2;
    //��һȦ�����������ܾ����Ƕ���
    for (int i = 0; i < CITY_SIZE; i++)
    {
        c1 = cities_permutation[i];
        if (i == CITY_SIZE - 1) //���һ�����к͵�һ�����м������
        {
            c2 = cities_permutation[0];
        }
        else
        {
            c2 = cities_permutation[i + 1];
        }
        total_distance += distance_2city(cities[c1], cities[c2]);
    }

    return total_distance;
}

//��ȡ�����������
void random_permutation(int * cities_permutation)
{
    int i, r, temp;
    for (i = 0; i < CITY_SIZE; i++)
    {
        cities_permutation[i] = i; //��ʼ���������У���ʼ��˳����
    }


    for (i = 0; i < CITY_SIZE; i++)
    {
        //��������˳���������
        r = rand() % (CITY_SIZE - i) + i;
        temp = cities_permutation[i];
        cities_permutation[i] = cities_permutation[r];
        cities_permutation[r] = temp;
    }
}




//�ߵ��������±�begin��end��Ԫ��λ��
void swap_element(int *p, int begin, int end)
{
    int temp;
    while (begin < end)
    {
        temp = p[begin];
        p[begin] = p[end];
        p[end] = temp;
        begin++;
        end--;
    }
}


//������ ��תindex_i <-> index_j ���Ԫ��
void two_opt_swap(int *cities_permutation, int *new_cities_permutation, int index_i, int index_j)
{
    for (int i = 0; i < CITY_SIZE; i++)
    {
        new_cities_permutation[i] = cities_permutation[i];
    }

    swap_element(new_cities_permutation, index_i, index_j);
}



int calc_delta(int i, int k,  int *tmp, CITIES * cities){
	int delta = 0;
	/*
                ���¼���˵����
                ����ÿ����������ת�Ժ�û��Ҫ�ٴ����¼����ܾ���
                ֻ��Ҫ�ڷ�ת��ͷβ����СС����

                ���磺
                �г�������   1-2-3-4-5 �ܾ��� = d12 + d23 + d34 + d45 + d51 = A
                ��ת������� 1-4-3-2-5 �ܾ��� = d14 + d43 + d32 + d25 + d51 = B
                ���� dij �� dji��һ���ģ�����BҲ���Ա�ʾ�� B = A - d12 - d45 + d14 + d25
                ������Ż����ǻ�������ԭ��
    */
	if (i == 0)
    {
        if (k == CITY_SIZE - 1)
        {
           delta = 0;
        }
        else
        {
            delta = 0
                - distance_2city(cities[tmp[k]], cities[tmp[k + 1]])
                + distance_2city(cities[tmp[i]], cities[tmp[k + 1]])
                - distance_2city(cities[tmp[CITY_SIZE - 1]], cities[tmp[i]])
                + distance_2city(cities[tmp[CITY_SIZE - 1]], cities[tmp[k]]);
        }

    }
    else
    {
        if (k == CITY_SIZE - 1)
        {
            delta = 0
                - distance_2city(cities[tmp[i - 1]], cities[tmp[i]])
                + distance_2city(cities[tmp[i - 1]], cities[tmp[k]])
                - distance_2city(cities[tmp[0]], cities[tmp[k]])
                + distance_2city(cities[tmp[i]], cities[tmp[0]]);
        }
        else
        {
            delta = 0
                - distance_2city(cities[tmp[i - 1]], cities[tmp[i]])
                + distance_2city(cities[tmp[i - 1]], cities[tmp[k]])
                - distance_2city(cities[tmp[k]], cities[tmp[k + 1]])
                + distance_2city(cities[tmp[i]], cities[tmp[k + 1]]);
        }
    }

    return delta;
}


/*
	ȥ�ش�������Delta������˵�����ڳ�������1-2-3-4-5-6-7-8-9-10�������3-5Ӧ�����������2-opt �� ��ʵ�϶���
	7-10֮��ķ�ת�ǲ���Ҫ�ظ�����ġ� ������Delta��ǰԤ����һ�¡�
	
	��Ȼ��������ļ��㱾����O��1�� �ģ���ʵ�ϲ�û�д���ʱ�临�Ӷȵļ��٣����²������������˸��Ӷȣ� 
	���delta���� ��O��n���ģ�����ȥ�ز���Ч�������Եġ� 
*/

void Update(int i, int k,  int *tmp, CITIES * cities){
	if (i && k != CITY_SIZE - 1){
		i --; k ++;
		for (int j = i; j <= k; j ++){
			for (int l = j + 1; l < CITY_SIZE; l ++){
				Delta[j][l] = calc_delta(j, l, tmp, cities);
			}
		}
	
		for (int j = 0; j < k; j ++){
			for (int l = i; l <= k; l ++){
				if (j >= l) continue;
				Delta[j][l] = calc_delta(j, l, tmp, cities);
			}
		}
	}// ������Ǳ߽磬����(i-1, k + 1)֮��� 
	else{
		for (i = 0; i < CITY_SIZE - 1; i++)
   		 {
		      for (k = i + 1; k < CITY_SIZE; k++)
			{
				Delta[i][k] = calc_delta(i, k, tmp, cities);
		      }
   		 }	
	}// �߽�Ҫ������� 

} 

//���ؾֲ��������߽����� max_no_improve
//best_solution���Ž�
//current_solution��ǰ��
void local_search(SOLUTION & best_solution, CITIES * cities, int max_no_improve)
{
    int count = 0;
    int i, k;

    int inital_cost = best_solution.cost; //��ʼ����

    int now_cost = 0;

    SOLUTION *current_solution = new SOLUTION; //Ϊ�˷�ֹ��ջ����ֱ��new�ˣ��㶮��
	
	for (i = 0; i < CITY_SIZE - 1; i++)
    {
       for (k = i + 1; k < CITY_SIZE; k++)
		{
			Delta[i][k] = calc_delta(i, k, best_solution.permutation, cities);
        }
    }
	
    do
    {
        //ö������
        for (i = 0; i < CITY_SIZE - 1; i++)
        {
            for (k = i + 1; k < CITY_SIZE; k++)
            {
                //������
                two_opt_swap(best_solution.permutation, current_solution->permutation, i, k);
				now_cost = inital_cost + Delta[i][k];
                current_solution->cost = now_cost;
                if (current_solution->cost < best_solution.cost)
                {
                    count = 0; //better cost found, so reset
                    for (int j = 0; j < CITY_SIZE; j++)
                    {
                        best_solution.permutation[j] = current_solution->permutation[j];
                    }
                    best_solution.cost = current_solution->cost;
                    inital_cost = best_solution.cost;
                    Update(i, k, best_solution.permutation, cities);
                }

            }
        }

        count++;

    } while (count <= max_no_improve);
}
//�жϽ���׼��
bool AcceptanceCriterion(int *cities_permutation, int *old_cities_permutation, CITIES * p_cities)
{
    int acceptance = 500; //��������,�뵱ǰ���������acceptance
    int old_cost = cost_total(old_cities_permutation, p_cities);
    int new_cost = cost_total(cities_permutation, p_cities);

    if ((new_cost <= (old_cost + acceptance)) || (new_cost >= (old_cost - acceptance)))
    {
        return true;
    }

    return false;
}

//���������зֳ�4�飬Ȼ�󰴿����´���˳��
//�����Ŷ�����
void double_bridge_move(int *cities_permutation, int * new_cities_permutation)
{
    int temp_perm[CITY_SIZE];

    int pos1 = 1 + rand() % (CITY_SIZE / 4);
    int pos2 = pos1 + 1 + rand() % (CITY_SIZE / 4);
    int pos3 = pos2 + 1 + rand() % (CITY_SIZE / 4);

    int i;
    vector<int> v;
    //��һ��
    for (i = 0; i < pos1; i++)
    {
        v.push_back(cities_permutation[i]);
    }

    //�ڶ���
    for (i = pos3; i < CITY_SIZE; i++)
    {
        v.push_back(cities_permutation[i]);
    }
    //������
    for (i = pos2; i < pos3; i++)
    {
        v.push_back(cities_permutation[i]);
    }

    //���Ŀ�
    for (i = pos1; i < pos2; i++)
    {
        v.push_back(cities_permutation[i]);
    }


    for (i = 0; i < (int)v.size(); i++)
    {
        temp_perm[i] = v[i];
    }
    //if accept�ж��Ƿ���ܵ�ǰ��
    if (AcceptanceCriterion(cities_permutation, temp_perm, cities))
    {
        memcpy(new_cities_permutation, temp_perm, sizeof(temp_perm));//accept
    }


}

//�Ŷ�
void perturbation(CITIES * cities, SOLUTION &best_solution, SOLUTION &current_solution)
{
    double_bridge_move(best_solution.permutation, current_solution.permutation);
    current_solution.cost = cost_total(current_solution.permutation, cities);
}

//��������
//max_iterations���ڵ�����������
//max_no_improve���ھֲ������߽�����
void iterated_local_search(SOLUTION & best_solution, CITIES * cities, int max_iterations, int max_no_improve)
{
    SOLUTION *current_solution = new SOLUTION;

    //��ó�ʼ�����
    random_permutation(best_solution.permutation);


    best_solution.cost = cost_total(best_solution.permutation, cities);
    local_search(best_solution, cities, max_no_improve); //��ʼ����

    for (int i = 0; i < max_iterations; i++)
    {
        perturbation(cities, best_solution, *current_solution); //�Ŷ�+�ж��Ƿ�����½�
        local_search(*current_solution, cities, max_no_improve);//�����ֲ�����

        //�ҵ����Ž�
        if (current_solution->cost < best_solution.cost)
        {
            for (int j = 0; j < CITY_SIZE; j++)
            {
                best_solution.permutation[j] = current_solution->permutation[j];
            }
            best_solution.cost = current_solution->cost;
        }
        cout << setw(13) << setiosflags(ios::left) <<"�������� " << i << " ��\t" << "���Ž� = " << best_solution.cost << " ��ǰ�� = " << current_solution->cost << endl;
    }

}
