/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

using System;
using System.Collections.Generic;
using Thrift;
using Thrift.Protocol;
using Thrift.Server;
using Thrift.Transport;


namespace higis
{
    public class CSharpClient
    {
        public static void Main()
        {
            try
            {
                var transport = new TFramedTransport(new TSocket("202.197.18.253", 4742));
                TProtocol protocol = new TBinaryProtocol(transport);
                var client = new HpgcJob.Client(protocol);

                transport.Open();

                var task1 = new Task
                 {
                     Id = 0,
                     Parents = new List<int>(),
                     Parent_count = 0,
                     Children = new List<int>(new int[] { 1 }),
                     Child_count = 1,
					 Process_count = 1,
                     Mpi_options = "",
                     Program_name = "test1",
                     Program_params = new List<string>(new string[] { "task1" }),
                     Program_param_count = 1
                 };

                var task2 = new Task
                {
                    Id = 1,
                    Parents = new List<int>(new int[] { 0 }),
                    Parent_count = 1,
                    Children = new List<int>(),
                    Child_count = 0,
					Process_count = 1,
                    Mpi_options = "",
                    Program_name = "test1",
                    Program_params = new List<string>(new string[] { "task2" }),
                    Program_param_count = 1
                };

                var job = new Job {Tasks = new List<Task> {task1, task2}};

                job.Task_count = job.Tasks.Count;
                job.Checksum = "blahblahblah";

                try
                {
                    JobResult result = client.run(job);
                    Console.WriteLine("Hpgc job has been executed.");
					Console.WriteLine("The status of the job is: " + result.Status.ToString());
					Console.WriteLine("The result body is: " + result.Result);
                }
                catch (HpgcAppException e)
                {
                    Console.WriteLine("hpgc app " + e.Name + " has got the exception " + e.Message);
                }

                double progress = client.get_progress(job);

                transport.Close();
            }
            catch (TApplicationException x)
            {
                Console.WriteLine(x.StackTrace);
            }

        }
    }
}
