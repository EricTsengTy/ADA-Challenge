#include <random>
#include <iostream>
#include <stdio.h>
#include <vector>
#define MAXV 20
class JOB
{
public:
	int matrix[MAXV][MAXV] = {{0}};
	int num_of_op;
};
std::default_random_engine generator(123);
std::uniform_real_distribution<float> rd(0, 1);
std::uniform_real_distribution<float> rd_s(1, 8);
std::uniform_real_distribution<float> rd_d(1, 96);
std::uniform_real_distribution<float> rd_w(0, 64);
void getDAG(JOB *job)
{
	for(int i = 0; i < job->num_of_op; i++)
		for(int j = i + 1; j < job->num_of_op; j++)
		{
			if(rd(generator) > 0.5)
				job->matrix[i][j] = 1;
		}
	return;
}
void printMa(JOB *job)
{
	for(int i = 0; i < job->num_of_op; i++)
	{	
		for(int j = 0; j < job->num_of_op; j++)
		{
			if(job->matrix[i][j])
				std::cout << 1 << " ";
			else
				std::cout << 0 << " ";
		}
		std::cout << std::endl;
	}
	return;	
}
int main(int argc, char const *argv[])
{
	FILE *fp;
	fp = fopen("test1.in", "w");
	int num_of_slices = 8;
	int num_of_jobs = 8;
	fprintf(fp, "%d\n", num_of_slices);
	fprintf(fp, "%d\n", num_of_jobs);
	JOB jobs[num_of_jobs];
	jobs[0].num_of_op = 13;
	jobs[1].num_of_op = 12;
	jobs[2].num_of_op = 4;
	jobs[3].num_of_op = 18;
	jobs[4].num_of_op = 10;
	jobs[5].num_of_op = 15;
	jobs[6].num_of_op = 9;
	jobs[7].num_of_op = 6;
	for(int i = 0; i < num_of_jobs; i++)
		getDAG(jobs + i);
	// for(int i = 0; i < num_of_jobs; i++)
	// 	printMa(jobs + i);
	for(int i = 0; i < num_of_jobs; i++)
	{
		fprintf(fp, "%d\n", jobs[i].num_of_op); //number of operations
		fprintf(fp, "%f\n", rd_w(generator)); //the weight of the job
		for(int j = 0; j < jobs[i].num_of_op; j++)
		{
			int num_of_dep = 0;
			for(int k = 0; k < jobs[i].num_of_op; k++)
			{
				if(jobs[i].matrix[j][k] == 1)
					num_of_dep++;
			}
			std::vector<int> output{(int)rd_s(generator), (int) rd_d(generator), num_of_dep};	
			//fprintf(fp, "%d %d %d ", (int)rd_s(generator), (int)rd_d(generator), num_of_dep);
			for(int k = 0; k < jobs[i].num_of_op; k++)
			{
				if(jobs[i].matrix[j][k] == 1)
					output.push_back(k);
					//fprintf(fp, "%d ", k);
			}
			fprintf(fp, "%d", output[0]);
			for (int i = 1; i < output.size(); ++i)
				fprintf(fp, " %d", output[i]);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
	return 0;
}
