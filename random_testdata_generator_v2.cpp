#include <random>
#include <iostream>
#include <stdio.h>
#include <deque>
#define MAXV 30
std::default_random_engine generator(123);
std::uniform_real_distribution<float> rd(0, 1);
std::uniform_real_distribution<float> rd_s(1, 8);
std::uniform_real_distribution<float> rd_d(1, 96);
std::uniform_real_distribution<float> rd_w(0, 64);

class JOB
{
public:
	int matrix[MAXV+5][MAXV+5];
	int num_of_op;
	void initDAG()
	{
		for(int i = 1; i <= MAXV; i++)
			for(int j = 1; j <= MAXV; j++)
				matrix[i][j] = 0;
		return;
	}
	void getDAG()
	{
		for(int i = 1; i <= num_of_op; i++)
			for(int j = i + 1; j <= num_of_op; j++)
			{
				if(rd(generator) > 0.5)
					matrix[i][j] = 1;
			}
		return;
	}
	void printMa()
	{
		for(int i = 1; i <= num_of_op; i++)
		{	
			for(int j = 1; j <= num_of_op; j++)
			{
				if(matrix[i][j] == 1)
					std::cout << 1 << " ";
				else
					std::cout << 0 << " ";
			}
			std::cout << std::endl;
		}
		return;	
	}
};

int main(int argc, char const *argv[])
{
	FILE *fp;
	fp = fopen("test2.in", "w");
	int num_of_slices = 8;
	int num_of_jobs = 8;
	fprintf(fp, "%d\n", num_of_slices);
	fprintf(fp, "%d\n", num_of_jobs);

	JOB jobs[num_of_jobs];
	for(int i = 0; i < num_of_jobs; i++)
		jobs[i].initDAG();

	jobs[0].num_of_op = 13;
	jobs[1].num_of_op = 12;
	jobs[2].num_of_op = 4;
	jobs[3].num_of_op = 18;
	jobs[4].num_of_op = 10;
	jobs[5].num_of_op = 15;
	jobs[6].num_of_op = 9;
	jobs[7].num_of_op = 6;

	for(int i = 0; i < num_of_jobs; i++)
		jobs[i].getDAG();
	// for(int i = 0; i < num_of_jobs; i++)
	// 	jobs[i].printMa();

	std::deque<int> q;
	// for(int k = 2 + 1; k <= jobs[0].num_of_op; k++)
	// 	if(jobs[0].matrix[2][k] == 1)
	// 		q.push_back(k);
	// std::cout << q.size() << std::endl;
	// while(q.size() > 1)
	// {
	// 	std::cout << q.front() << " ";
	// 	q.pop_front();
	// }
	// std::cout << q.front() << std::endl;
	// 	q.pop_front();
	 int num_of_dep;
	for(int i = 0; i < num_of_jobs; i++)
	{
		fprintf(fp, "%d\n", jobs[i].num_of_op); //number of operations
		fprintf(fp, "%f\n", rd_w(generator)); //the weight of the job
		for(int j = 1; j <= jobs[i].num_of_op; j++)
		{
			for(int k = j + 1; k <= jobs[i].num_of_op; k++)
			{
				if(jobs[i].matrix[j][k] == 1)
				{
					q.push_back(k);
				}
			}
			num_of_dep = q.size();
			fprintf(fp, "%d %d %d", (int)rd_s(generator), (int)rd_d(generator), num_of_dep);
			while(num_of_dep > 0)
			{
				fprintf(fp, " %d", q.front());
				q.pop_front();
				num_of_dep--;
			}
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
	return 0;
}