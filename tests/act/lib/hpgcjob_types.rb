#
# Autogenerated by Thrift Compiler (0.7.0)
#
# DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
#


module ParallelEnv
  MPI = 1
  OPENMP = 2
  CUDA = 3
  MAPREDUCE = 4
  VALUE_MAP = {1 => "MPI", 2 => "OPENMP", 3 => "CUDA", 4 => "MAPREDUCE"}
  VALID_VALUES = Set.new([MPI, OPENMP, CUDA, MAPREDUCE]).freeze
end

class Context
  include ::Thrift::Struct, ::Thrift::Struct_Union
  PARALLEL_ENV = 1
  OPTIONS = 2

  FIELDS = {
    PARALLEL_ENV => {:type => ::Thrift::Types::I32, :name => 'parallel_env', :enum_class => ParallelEnv},
    OPTIONS => {:type => ::Thrift::Types::MAP, :name => 'options', :key => {:type => ::Thrift::Types::STRING}, :value => {:type => ::Thrift::Types::STRING}}
  }

  def struct_fields; FIELDS; end

  def validate
    unless @parallel_env.nil? || ParallelEnv::VALID_VALUES.include?(@parallel_env)
      raise ::Thrift::ProtocolException.new(::Thrift::ProtocolException::UNKNOWN, 'Invalid value of field parallel_env!')
    end
  end

  ::Thrift::Struct.generate_accessors self
end

class Job
  include ::Thrift::Struct, ::Thrift::Struct_Union
  ID = 1
  PARENTS = 2
  PARENT_COUNT = 3
  CHILDREN = 4
  CHILD_COUNT = 5
  APP_ID = 6
  APP_OPTIONS = 7
  RUNTIME_CONTEXT = 8

  FIELDS = {
    ID => {:type => ::Thrift::Types::I32, :name => 'id'},
    PARENTS => {:type => ::Thrift::Types::LIST, :name => 'parents', :element => {:type => ::Thrift::Types::I32}},
    PARENT_COUNT => {:type => ::Thrift::Types::I32, :name => 'parent_count'},
    CHILDREN => {:type => ::Thrift::Types::LIST, :name => 'children', :element => {:type => ::Thrift::Types::I32}},
    CHILD_COUNT => {:type => ::Thrift::Types::I32, :name => 'child_count'},
    APP_ID => {:type => ::Thrift::Types::I32, :name => 'app_id'},
    APP_OPTIONS => {:type => ::Thrift::Types::MAP, :name => 'app_options', :key => {:type => ::Thrift::Types::STRING}, :value => {:type => ::Thrift::Types::STRING}},
    RUNTIME_CONTEXT => {:type => ::Thrift::Types::STRUCT, :name => 'runtime_context', :class => Context}
  }

  def struct_fields; FIELDS; end

  def validate
  end

  ::Thrift::Struct.generate_accessors self
end

class Result
  include ::Thrift::Struct, ::Thrift::Struct_Union
  STATUS = 1
  MESSAGE = 2
  PROGRESS = 3

  FIELDS = {
    STATUS => {:type => ::Thrift::Types::I32, :name => 'status'},
    MESSAGE => {:type => ::Thrift::Types::STRING, :name => 'message'},
    PROGRESS => {:type => ::Thrift::Types::DOUBLE, :name => 'progress'}
  }

  def struct_fields; FIELDS; end

  def validate
  end

  ::Thrift::Struct.generate_accessors self
end

class JobFlow
  include ::Thrift::Struct, ::Thrift::Struct_Union
  JOBS = 1
  JOB_COUNT = 2

  FIELDS = {
    JOBS => {:type => ::Thrift::Types::LIST, :name => 'jobs', :element => {:type => ::Thrift::Types::STRUCT, :class => Job}},
    JOB_COUNT => {:type => ::Thrift::Types::I32, :name => 'job_count'}
  }

  def struct_fields; FIELDS; end

  def validate
  end

  ::Thrift::Struct.generate_accessors self
end

class HpgcJobException < ::Thrift::Exception
  include ::Thrift::Struct, ::Thrift::Struct_Union
  APP_ID = 1
  NAME = 2
  MESSAGE = 3

  FIELDS = {
    APP_ID => {:type => ::Thrift::Types::I32, :name => 'app_id'},
    NAME => {:type => ::Thrift::Types::STRING, :name => 'name'},
    MESSAGE => {:type => ::Thrift::Types::STRING, :name => 'message'}
  }

  def struct_fields; FIELDS; end

  def validate
  end

  ::Thrift::Struct.generate_accessors self
end

