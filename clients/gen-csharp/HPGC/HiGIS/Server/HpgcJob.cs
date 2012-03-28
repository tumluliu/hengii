/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Thrift;
using Thrift.Collections;
using Thrift.Protocol;
using Thrift.Transport;
namespace HPGC.HiGIS.Server
{
  public class HpgcJob {
    public interface Iface {
      int start_single_job(Job job);
      int start(JobFlow flow);
      void pause(int client_ticket);
      void resume(int client_ticket);
      void cancel(int client_ticket);
      Result get_status(int client_ticket);
    }

    public class Client : Iface {
      public Client(TProtocol prot) : this(prot, prot)
      {
      }

      public Client(TProtocol iprot, TProtocol oprot)
      {
        iprot_ = iprot;
        oprot_ = oprot;
      }

      protected TProtocol iprot_;
      protected TProtocol oprot_;
      protected int seqid_;

      public TProtocol InputProtocol
      {
        get { return iprot_; }
      }
      public TProtocol OutputProtocol
      {
        get { return oprot_; }
      }


      public int start_single_job(Job job)
      {
        send_start_single_job(job);
        return recv_start_single_job();
      }

      public void send_start_single_job(Job job)
      {
        oprot_.WriteMessageBegin(new TMessage("start_single_job", TMessageType.Call, seqid_));
        start_single_job_args args = new start_single_job_args();
        args.Job = job;
        args.Write(oprot_);
        oprot_.WriteMessageEnd();
        oprot_.Transport.Flush();
      }

      public int recv_start_single_job()
      {
        TMessage msg = iprot_.ReadMessageBegin();
        if (msg.Type == TMessageType.Exception) {
          TApplicationException x = TApplicationException.Read(iprot_);
          iprot_.ReadMessageEnd();
          throw x;
        }
        start_single_job_result result = new start_single_job_result();
        result.Read(iprot_);
        iprot_.ReadMessageEnd();
        if (result.__isset.success) {
          return result.Success;
        }
        if (result.__isset.e) {
          throw result.E;
        }
        throw new TApplicationException(TApplicationException.ExceptionType.MissingResult, "start_single_job failed: unknown result");
      }

      public int start(JobFlow flow)
      {
        send_start(flow);
        return recv_start();
      }

      public void send_start(JobFlow flow)
      {
        oprot_.WriteMessageBegin(new TMessage("start", TMessageType.Call, seqid_));
        start_args args = new start_args();
        args.Flow = flow;
        args.Write(oprot_);
        oprot_.WriteMessageEnd();
        oprot_.Transport.Flush();
      }

      public int recv_start()
      {
        TMessage msg = iprot_.ReadMessageBegin();
        if (msg.Type == TMessageType.Exception) {
          TApplicationException x = TApplicationException.Read(iprot_);
          iprot_.ReadMessageEnd();
          throw x;
        }
        start_result result = new start_result();
        result.Read(iprot_);
        iprot_.ReadMessageEnd();
        if (result.__isset.success) {
          return result.Success;
        }
        if (result.__isset.e) {
          throw result.E;
        }
        throw new TApplicationException(TApplicationException.ExceptionType.MissingResult, "start failed: unknown result");
      }

      public void pause(int client_ticket)
      {
        send_pause(client_ticket);
        recv_pause();
      }

      public void send_pause(int client_ticket)
      {
        oprot_.WriteMessageBegin(new TMessage("pause", TMessageType.Call, seqid_));
        pause_args args = new pause_args();
        args.Client_ticket = client_ticket;
        args.Write(oprot_);
        oprot_.WriteMessageEnd();
        oprot_.Transport.Flush();
      }

      public void recv_pause()
      {
        TMessage msg = iprot_.ReadMessageBegin();
        if (msg.Type == TMessageType.Exception) {
          TApplicationException x = TApplicationException.Read(iprot_);
          iprot_.ReadMessageEnd();
          throw x;
        }
        pause_result result = new pause_result();
        result.Read(iprot_);
        iprot_.ReadMessageEnd();
        return;
      }

      public void resume(int client_ticket)
      {
        send_resume(client_ticket);
        recv_resume();
      }

      public void send_resume(int client_ticket)
      {
        oprot_.WriteMessageBegin(new TMessage("resume", TMessageType.Call, seqid_));
        resume_args args = new resume_args();
        args.Client_ticket = client_ticket;
        args.Write(oprot_);
        oprot_.WriteMessageEnd();
        oprot_.Transport.Flush();
      }

      public void recv_resume()
      {
        TMessage msg = iprot_.ReadMessageBegin();
        if (msg.Type == TMessageType.Exception) {
          TApplicationException x = TApplicationException.Read(iprot_);
          iprot_.ReadMessageEnd();
          throw x;
        }
        resume_result result = new resume_result();
        result.Read(iprot_);
        iprot_.ReadMessageEnd();
        return;
      }

      public void cancel(int client_ticket)
      {
        send_cancel(client_ticket);
        recv_cancel();
      }

      public void send_cancel(int client_ticket)
      {
        oprot_.WriteMessageBegin(new TMessage("cancel", TMessageType.Call, seqid_));
        cancel_args args = new cancel_args();
        args.Client_ticket = client_ticket;
        args.Write(oprot_);
        oprot_.WriteMessageEnd();
        oprot_.Transport.Flush();
      }

      public void recv_cancel()
      {
        TMessage msg = iprot_.ReadMessageBegin();
        if (msg.Type == TMessageType.Exception) {
          TApplicationException x = TApplicationException.Read(iprot_);
          iprot_.ReadMessageEnd();
          throw x;
        }
        cancel_result result = new cancel_result();
        result.Read(iprot_);
        iprot_.ReadMessageEnd();
        return;
      }

      public Result get_status(int client_ticket)
      {
        send_get_status(client_ticket);
        return recv_get_status();
      }

      public void send_get_status(int client_ticket)
      {
        oprot_.WriteMessageBegin(new TMessage("get_status", TMessageType.Call, seqid_));
        get_status_args args = new get_status_args();
        args.Client_ticket = client_ticket;
        args.Write(oprot_);
        oprot_.WriteMessageEnd();
        oprot_.Transport.Flush();
      }

      public Result recv_get_status()
      {
        TMessage msg = iprot_.ReadMessageBegin();
        if (msg.Type == TMessageType.Exception) {
          TApplicationException x = TApplicationException.Read(iprot_);
          iprot_.ReadMessageEnd();
          throw x;
        }
        get_status_result result = new get_status_result();
        result.Read(iprot_);
        iprot_.ReadMessageEnd();
        if (result.__isset.success) {
          return result.Success;
        }
        throw new TApplicationException(TApplicationException.ExceptionType.MissingResult, "get_status failed: unknown result");
      }

    }
    public class Processor : TProcessor {
      public Processor(Iface iface)
      {
        iface_ = iface;
        processMap_["start_single_job"] = start_single_job_Process;
        processMap_["start"] = start_Process;
        processMap_["pause"] = pause_Process;
        processMap_["resume"] = resume_Process;
        processMap_["cancel"] = cancel_Process;
        processMap_["get_status"] = get_status_Process;
      }

      protected delegate void ProcessFunction(int seqid, TProtocol iprot, TProtocol oprot);
      private Iface iface_;
      protected Dictionary<string, ProcessFunction> processMap_ = new Dictionary<string, ProcessFunction>();

      public bool Process(TProtocol iprot, TProtocol oprot)
      {
        try
        {
          TMessage msg = iprot.ReadMessageBegin();
          ProcessFunction fn;
          processMap_.TryGetValue(msg.Name, out fn);
          if (fn == null) {
            TProtocolUtil.Skip(iprot, TType.Struct);
            iprot.ReadMessageEnd();
            TApplicationException x = new TApplicationException (TApplicationException.ExceptionType.UnknownMethod, "Invalid method name: '" + msg.Name + "'");
            oprot.WriteMessageBegin(new TMessage(msg.Name, TMessageType.Exception, msg.SeqID));
            x.Write(oprot);
            oprot.WriteMessageEnd();
            oprot.Transport.Flush();
            return true;
          }
          fn(msg.SeqID, iprot, oprot);
        }
        catch (IOException)
        {
          return false;
        }
        return true;
      }

      public void start_single_job_Process(int seqid, TProtocol iprot, TProtocol oprot)
      {
        start_single_job_args args = new start_single_job_args();
        args.Read(iprot);
        iprot.ReadMessageEnd();
        start_single_job_result result = new start_single_job_result();
        try {
          result.Success = iface_.start_single_job(args.Job);
        } catch (HpgcJobException e) {
          result.E = e;
        }
        oprot.WriteMessageBegin(new TMessage("start_single_job", TMessageType.Reply, seqid)); 
        result.Write(oprot);
        oprot.WriteMessageEnd();
        oprot.Transport.Flush();
      }

      public void start_Process(int seqid, TProtocol iprot, TProtocol oprot)
      {
        start_args args = new start_args();
        args.Read(iprot);
        iprot.ReadMessageEnd();
        start_result result = new start_result();
        try {
          result.Success = iface_.start(args.Flow);
        } catch (HpgcJobException e) {
          result.E = e;
        }
        oprot.WriteMessageBegin(new TMessage("start", TMessageType.Reply, seqid)); 
        result.Write(oprot);
        oprot.WriteMessageEnd();
        oprot.Transport.Flush();
      }

      public void pause_Process(int seqid, TProtocol iprot, TProtocol oprot)
      {
        pause_args args = new pause_args();
        args.Read(iprot);
        iprot.ReadMessageEnd();
        pause_result result = new pause_result();
        iface_.pause(args.Client_ticket);
        oprot.WriteMessageBegin(new TMessage("pause", TMessageType.Reply, seqid)); 
        result.Write(oprot);
        oprot.WriteMessageEnd();
        oprot.Transport.Flush();
      }

      public void resume_Process(int seqid, TProtocol iprot, TProtocol oprot)
      {
        resume_args args = new resume_args();
        args.Read(iprot);
        iprot.ReadMessageEnd();
        resume_result result = new resume_result();
        iface_.resume(args.Client_ticket);
        oprot.WriteMessageBegin(new TMessage("resume", TMessageType.Reply, seqid)); 
        result.Write(oprot);
        oprot.WriteMessageEnd();
        oprot.Transport.Flush();
      }

      public void cancel_Process(int seqid, TProtocol iprot, TProtocol oprot)
      {
        cancel_args args = new cancel_args();
        args.Read(iprot);
        iprot.ReadMessageEnd();
        cancel_result result = new cancel_result();
        iface_.cancel(args.Client_ticket);
        oprot.WriteMessageBegin(new TMessage("cancel", TMessageType.Reply, seqid)); 
        result.Write(oprot);
        oprot.WriteMessageEnd();
        oprot.Transport.Flush();
      }

      public void get_status_Process(int seqid, TProtocol iprot, TProtocol oprot)
      {
        get_status_args args = new get_status_args();
        args.Read(iprot);
        iprot.ReadMessageEnd();
        get_status_result result = new get_status_result();
        result.Success = iface_.get_status(args.Client_ticket);
        oprot.WriteMessageBegin(new TMessage("get_status", TMessageType.Reply, seqid)); 
        result.Write(oprot);
        oprot.WriteMessageEnd();
        oprot.Transport.Flush();
      }

    }


    [Serializable]
    public partial class start_single_job_args : TBase
    {
      private Job _job;

      public Job Job
      {
        get
        {
          return _job;
        }
        set
        {
          __isset.job = true;
          this._job = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool job;
      }

      public start_single_job_args() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 1:
              if (field.Type == TType.Struct) {
                Job = new Job();
                Job.Read(iprot);
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("start_single_job_args");
        oprot.WriteStructBegin(struc);
        TField field = new TField();
        if (Job != null && __isset.job) {
          field.Name = "job";
          field.Type = TType.Struct;
          field.ID = 1;
          oprot.WriteFieldBegin(field);
          Job.Write(oprot);
          oprot.WriteFieldEnd();
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("start_single_job_args(");
        sb.Append("Job: ");
        sb.Append(Job== null ? "<null>" : Job.ToString());
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class start_single_job_result : TBase
    {
      private int _success;
      private HpgcJobException _e;

      public int Success
      {
        get
        {
          return _success;
        }
        set
        {
          __isset.success = true;
          this._success = value;
        }
      }

      public HpgcJobException E
      {
        get
        {
          return _e;
        }
        set
        {
          __isset.e = true;
          this._e = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool success;
        public bool e;
      }

      public start_single_job_result() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 0:
              if (field.Type == TType.I32) {
                Success = iprot.ReadI32();
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            case 1:
              if (field.Type == TType.Struct) {
                E = new HpgcJobException();
                E.Read(iprot);
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("start_single_job_result");
        oprot.WriteStructBegin(struc);
        TField field = new TField();

        if (this.__isset.success) {
          field.Name = "Success";
          field.Type = TType.I32;
          field.ID = 0;
          oprot.WriteFieldBegin(field);
          oprot.WriteI32(Success);
          oprot.WriteFieldEnd();
        } else if (this.__isset.e) {
          if (E != null) {
            field.Name = "E";
            field.Type = TType.Struct;
            field.ID = 1;
            oprot.WriteFieldBegin(field);
            E.Write(oprot);
            oprot.WriteFieldEnd();
          }
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("start_single_job_result(");
        sb.Append("Success: ");
        sb.Append(Success);
        sb.Append(",E: ");
        sb.Append(E== null ? "<null>" : E.ToString());
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class start_args : TBase
    {
      private JobFlow _flow;

      public JobFlow Flow
      {
        get
        {
          return _flow;
        }
        set
        {
          __isset.flow = true;
          this._flow = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool flow;
      }

      public start_args() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 1:
              if (field.Type == TType.Struct) {
                Flow = new JobFlow();
                Flow.Read(iprot);
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("start_args");
        oprot.WriteStructBegin(struc);
        TField field = new TField();
        if (Flow != null && __isset.flow) {
          field.Name = "flow";
          field.Type = TType.Struct;
          field.ID = 1;
          oprot.WriteFieldBegin(field);
          Flow.Write(oprot);
          oprot.WriteFieldEnd();
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("start_args(");
        sb.Append("Flow: ");
        sb.Append(Flow== null ? "<null>" : Flow.ToString());
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class start_result : TBase
    {
      private int _success;
      private HpgcJobException _e;

      public int Success
      {
        get
        {
          return _success;
        }
        set
        {
          __isset.success = true;
          this._success = value;
        }
      }

      public HpgcJobException E
      {
        get
        {
          return _e;
        }
        set
        {
          __isset.e = true;
          this._e = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool success;
        public bool e;
      }

      public start_result() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 0:
              if (field.Type == TType.I32) {
                Success = iprot.ReadI32();
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            case 1:
              if (field.Type == TType.Struct) {
                E = new HpgcJobException();
                E.Read(iprot);
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("start_result");
        oprot.WriteStructBegin(struc);
        TField field = new TField();

        if (this.__isset.success) {
          field.Name = "Success";
          field.Type = TType.I32;
          field.ID = 0;
          oprot.WriteFieldBegin(field);
          oprot.WriteI32(Success);
          oprot.WriteFieldEnd();
        } else if (this.__isset.e) {
          if (E != null) {
            field.Name = "E";
            field.Type = TType.Struct;
            field.ID = 1;
            oprot.WriteFieldBegin(field);
            E.Write(oprot);
            oprot.WriteFieldEnd();
          }
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("start_result(");
        sb.Append("Success: ");
        sb.Append(Success);
        sb.Append(",E: ");
        sb.Append(E== null ? "<null>" : E.ToString());
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class pause_args : TBase
    {
      private int _client_ticket;

      public int Client_ticket
      {
        get
        {
          return _client_ticket;
        }
        set
        {
          __isset.client_ticket = true;
          this._client_ticket = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool client_ticket;
      }

      public pause_args() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 1:
              if (field.Type == TType.I32) {
                Client_ticket = iprot.ReadI32();
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("pause_args");
        oprot.WriteStructBegin(struc);
        TField field = new TField();
        if (__isset.client_ticket) {
          field.Name = "client_ticket";
          field.Type = TType.I32;
          field.ID = 1;
          oprot.WriteFieldBegin(field);
          oprot.WriteI32(Client_ticket);
          oprot.WriteFieldEnd();
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("pause_args(");
        sb.Append("Client_ticket: ");
        sb.Append(Client_ticket);
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class pause_result : TBase
    {

      public pause_result() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("pause_result");
        oprot.WriteStructBegin(struc);

        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("pause_result(");
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class resume_args : TBase
    {
      private int _client_ticket;

      public int Client_ticket
      {
        get
        {
          return _client_ticket;
        }
        set
        {
          __isset.client_ticket = true;
          this._client_ticket = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool client_ticket;
      }

      public resume_args() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 1:
              if (field.Type == TType.I32) {
                Client_ticket = iprot.ReadI32();
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("resume_args");
        oprot.WriteStructBegin(struc);
        TField field = new TField();
        if (__isset.client_ticket) {
          field.Name = "client_ticket";
          field.Type = TType.I32;
          field.ID = 1;
          oprot.WriteFieldBegin(field);
          oprot.WriteI32(Client_ticket);
          oprot.WriteFieldEnd();
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("resume_args(");
        sb.Append("Client_ticket: ");
        sb.Append(Client_ticket);
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class resume_result : TBase
    {

      public resume_result() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("resume_result");
        oprot.WriteStructBegin(struc);

        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("resume_result(");
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class cancel_args : TBase
    {
      private int _client_ticket;

      public int Client_ticket
      {
        get
        {
          return _client_ticket;
        }
        set
        {
          __isset.client_ticket = true;
          this._client_ticket = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool client_ticket;
      }

      public cancel_args() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 1:
              if (field.Type == TType.I32) {
                Client_ticket = iprot.ReadI32();
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("cancel_args");
        oprot.WriteStructBegin(struc);
        TField field = new TField();
        if (__isset.client_ticket) {
          field.Name = "client_ticket";
          field.Type = TType.I32;
          field.ID = 1;
          oprot.WriteFieldBegin(field);
          oprot.WriteI32(Client_ticket);
          oprot.WriteFieldEnd();
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("cancel_args(");
        sb.Append("Client_ticket: ");
        sb.Append(Client_ticket);
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class cancel_result : TBase
    {

      public cancel_result() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("cancel_result");
        oprot.WriteStructBegin(struc);

        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("cancel_result(");
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class get_status_args : TBase
    {
      private int _client_ticket;

      public int Client_ticket
      {
        get
        {
          return _client_ticket;
        }
        set
        {
          __isset.client_ticket = true;
          this._client_ticket = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool client_ticket;
      }

      public get_status_args() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 1:
              if (field.Type == TType.I32) {
                Client_ticket = iprot.ReadI32();
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("get_status_args");
        oprot.WriteStructBegin(struc);
        TField field = new TField();
        if (__isset.client_ticket) {
          field.Name = "client_ticket";
          field.Type = TType.I32;
          field.ID = 1;
          oprot.WriteFieldBegin(field);
          oprot.WriteI32(Client_ticket);
          oprot.WriteFieldEnd();
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("get_status_args(");
        sb.Append("Client_ticket: ");
        sb.Append(Client_ticket);
        sb.Append(")");
        return sb.ToString();
      }

    }


    [Serializable]
    public partial class get_status_result : TBase
    {
      private Result _success;

      public Result Success
      {
        get
        {
          return _success;
        }
        set
        {
          __isset.success = true;
          this._success = value;
        }
      }


      public Isset __isset;
      [Serializable]
      public struct Isset {
        public bool success;
      }

      public get_status_result() {
      }

      public void Read (TProtocol iprot)
      {
        TField field;
        iprot.ReadStructBegin();
        while (true)
        {
          field = iprot.ReadFieldBegin();
          if (field.Type == TType.Stop) { 
            break;
          }
          switch (field.ID)
          {
            case 0:
              if (field.Type == TType.Struct) {
                Success = new Result();
                Success.Read(iprot);
              } else { 
                TProtocolUtil.Skip(iprot, field.Type);
              }
              break;
            default: 
              TProtocolUtil.Skip(iprot, field.Type);
              break;
          }
          iprot.ReadFieldEnd();
        }
        iprot.ReadStructEnd();
      }

      public void Write(TProtocol oprot) {
        TStruct struc = new TStruct("get_status_result");
        oprot.WriteStructBegin(struc);
        TField field = new TField();

        if (this.__isset.success) {
          if (Success != null) {
            field.Name = "Success";
            field.Type = TType.Struct;
            field.ID = 0;
            oprot.WriteFieldBegin(field);
            Success.Write(oprot);
            oprot.WriteFieldEnd();
          }
        }
        oprot.WriteFieldStop();
        oprot.WriteStructEnd();
      }

      public override string ToString() {
        StringBuilder sb = new StringBuilder("get_status_result(");
        sb.Append("Success: ");
        sb.Append(Success== null ? "<null>" : Success.ToString());
        sb.Append(")");
        return sb.ToString();
      }

    }

  }
}
