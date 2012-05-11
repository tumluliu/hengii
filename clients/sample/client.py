#!/usr/bin/env python

# The client sample written by python.
#
# by YANG Anran at 2012.04.19 21:33
# 
# Some server-side and env var preparations are needed, however you can reuse
# the test framework to make it rather easy:
# 1. suppose you are in the higine root dir(contains subdirs: server, client, etc.)
# 2. use "source settestenv" to load test environments
# 3. use "_prepare_program inchar" and "_prepare_program outchar" to prepare the 2 programs 
# 4. use "higine" to start server, which is actually in <higine root>/tests/.live/
# 5. run this script, and see the result both on client and server sides
#
# If success, you can see an 'X' in the result section

import sys
sys.path.append('../gen-py')

from hpgcjob import HpgcJob
from hpgcjob.ttypes import *

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

try:

	# Make socket
  transport = TSocket.TSocket('localhost', 9990)

  # Buffering is critical. Raw sockets are very slow
  transport = TTransport.TFramedTransport(transport)

  # Wrap in a protocol
  protocol = TBinaryProtocol.TBinaryProtocol(transport)

  # Create a client to use the protocol encoder
  client = HpgcJob.Client(protocol)

  # Connect!
  transport.open()

  # create an empty job flow
  job_flow = JobFlow()
  job_flow.jobs      = []
  job_flow.job_count = 0
  
  # add job 1
  job1              = Job()
  job1.id           = 0
  job1.parents      = []
  job1.parent_count = 0
  job1.children     = [1]
  job1.child_count  = len(job1.children)
  job1.app_uri = "outchar"
  
  job1.runtime_context              = Context()
  job1.runtime_context.parallel_env = ParallelEnv.MPI
  job1.runtime_context.options      = { "process_count": "1" }
  
  job1.app_options = { "output": "~/tmp.txt" }
  
  job_flow.jobs.append(job1);
  job_flow.job_count += 1;
  
  # add job2, whose input is job1's output, so specified as child of job1
  job2              = Job()
  job2.id           = 1
  job2.parents      = [0]
  job2.parent_count = len(job2.parents)
  job2.children     = []
  job2.child_count  = len(job2.children)
  job2.app_uri = "inchar"
  
  job2.runtime_context              = Context()
  job2.runtime_context.parallel_env = ParallelEnv.MPI
  job2.runtime_context.options      = { "process_count": "1" }
  
  job2.app_options = { "input": "~/tmp.txt" }
  
  job_flow.jobs.append(job2);
  job_flow.job_count += 1;
  
  # start job flow with user name "sample",
  # the id is a "ticket" for latter operation
  id = client.start(job_flow, "sample")
  
  # block until the server complete the job flow or return an error 
  ret = client.get_status(id)
  while (ret.flow_status != Status.FINISHED and ret.flow_status != Status.FAILED and ret.flow_status != Status.NOT_EXIST):
  	ret = client.get_status(id)
  
  # print result, first the message of jobflow, then the messages of each job
  print "The flow has reached: " + str(ret.flow_status)
  print "result is: "
  print ret.message
  
  for job_ret in ret.job_result_list:
  	print job_ret.message

  # Close!
  transport.close()

except Thrift.TException, tx:
	print '%s' % (tx.message)
