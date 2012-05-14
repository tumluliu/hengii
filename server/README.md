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
* all player identifier is its id, unless given "by index", e.g. get_index(0) means get the player whose id is 0
* methods who returns int as fail/success signal obey the convention that 0 means no problem, while negative numbers are oftern used to indicate failure
* when concating string, every procedure put one space in the END

## IMPLEMENT DICIPLINE(may not sufficient, but helps reducing errors)
* ALWAYS send event at last, do nothing afterwards
