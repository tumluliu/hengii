## QUALITY SPECIFICATIONS
* never "using" in header files, never "using namespace"
* with -Wall & -Wextra & -Weffc++, no warning in OUR codes 
* using comments prefixed by "WARNING" to identify potential problems

## NAMING CONVENTIONS
* with no special announcement, use simple word to idicate classes working with flow, e.g. Tracker
* if possible confusion exists, use "flow" prefix to indicate classes working with flow, e.g. FlowRuntime 
* use "job" prefix to indicate classes working with job, e.g. JobTracker
* use "Go" prefix on methods to indicate asynchronose methods, while the one without it is the synchronose version, e.g. GoPlay & Play

## DESIGN CONVENTION
* all player identifier is its id, unless given "by index", e.g. get\_index(0) means get the player whose id is 0
* methods who returns int as fail/success signal obey the convention that 0 means no problem, while negative numbers are oftern used to indicate failure
* when concating string, every procedure put one space in the END
* using 'out\_' prefix to indicate out paramters, and always put these parameters in front
* using 'p' prefix to indicate pointer variables, especially when the name is ambigious
_(the last two has not been applied to all codes yet)_
* when holding a object, using unique\_ptr to describe absolute ownership
* when holding a object, using (const if possible)reference to describe using privilledge(paired with unique\_ptr)
* when holding a object, if using reference to describe using privilledge is not possible e.g. some functions return it out, it should be stored in containers, or it is nullable, using weak\_ptr instead
* when holding a object, if having to use weak\_ptr, using shared\_ptr instead of unique\_ptr
* when holding a object, only directly choose shared\_ptr when the resource is really belongs to several, and remember to check if circular dependencies exist 
_(the last five may be arguable because of the ambigious meaning of passing a reference as a parameter. However, you cannot hold a passed-in reference except in constructor, so the situation may not be that bad)_
* when a function contains a reference out param, it is not responsible to tell how the ORIGINAL value of the reference will be updated or simply throw out, so NEVER pass a on-heap variable as out reference param 

## IMPLEMENT DICIPLINE 
* always send event at last, do nothing afterwards
* never use RAW pointers unless there are explicit reasonable reasons (refer to impl of Validator)
* when use reference, use const ones as possible
* never return a reference except for copy constructor
* when passing parameters, never use pass-by-value unless it is basic types
* when passing parameters, if the caller won't hold it, use reference
* add 'explicit' to one parameter constructor to avoid unexpected implicit cast(from google style)
_(the last one has not been applied to all codes yet)_
