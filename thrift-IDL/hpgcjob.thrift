/*
 * Licensed to the DataBase Research Group (DBRG) of National University
 * of Defense Technology (NUDT).
 *
 * Author: Lu LIU
 * Date: 2011/10/13
 * Description: This is the interface description file of the HPGC Job 
 * 				processing service interface of HiGIS server (hpgcjobserver).
 *				It consists of a Job struct and a JobFlow struct.
 */

namespace cpp HPGC.HiGIS.Server 
namespace csharp HPGC.HiGIS.Server
namespace java HPGC.HiGIS.Server

enum ParallelEnv
{
	MPI = 1,
	OPENMP = 2,
	CUDA = 3,
	MAPREDUCE = 4
}

struct Context
{
	1: ParallelEnv parallel_env,
	2: map<string,string> options
}

struct Job 
{
	1: i32 id,
	2: list<i32> parents,
	3: i32 parent_count,
	4: list<i32> children,
	5: i32 child_count,
	6: i32 app_id,
	7: map<string,string> app_options,
	8: Context runtime_context 
}

struct Result 
{
	1: i32 status,
	2: string message,
	3: double progress
}

struct JobFlow
{
	1: list<Job> jobs,
	2: i32 job_count
}

exception HpgcJobException
{
	1: i32 app_id,
	2: string name,
	3: string message
}

service HpgcJob
{
	i32 start_single_job(1:Job job) throws (1:HpgcJobException e),
	i32 start(1:JobFlow flow) throws (1:HpgcJobException e),
	void pause(1:i32 client_ticket),
	void resume(1:i32 client_ticket),
	void cancel(1:i32 client_ticket),
	Result get_status(1:i32 client_ticket),
}
