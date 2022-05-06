# Summary

The primary focus of this library is to create a method of writing log data to multiple locations at once. Each with their own formatting and listening levels defined. There are a few objects that achieve this. The `Logger` object is the most basic. It is responsible for taking the message and formatting it correctly. A `Probe` attaches to a `Logger` and handles directing the output to some form of presentation. By default I have defined `Probe`s for stdout and fileout. You can create custom probes by inheriting from the `OutputWrapper` abstract class.

`Logger`s can also be combined together using an `Aggregate`. The `Aggregate` deligates output to each of the logs associated with it. This allows you to easily write to multiple logs, which also might contain multiple probes, all with different potentially different formatting preferences. 

Prolly got some bug fixing to do.
# Examples
## Logging
```c++
#include "log.h"

int main(..)
{
    // Creating a logger should be done by requesting from the library
    // if a log exists with this name it will return it
    // if not it will create the log with this name.
    // This means once we set up the loggers the settings will stay
    // and we can globally request them at any time
    
    // This means we can set up the log once and just request it whenever c:
    Log::Logger& main = Log::getLog("main");
    Log::Behaviour& mainPref = main.getBehaviour();

    // In order to see any of the output we have to attach a probe
    // The logger maintains the lifetime of the probes.
    // but it will return a handle to the newly created probe.
    // By default I have defined two wrappers. StdOut, and FileOut
    auto p_handle = mainPref.addProbe(new Log::Wrapper::StdOut());
    main << "This will be written to stream";
    
    mainPref.addProbe(new Log::Wrapper::FileOut("logging/output.log"));
    main << "This will be written to stream, AND file";
    
    // if you want to delete a probe, you hold can delete the handle
    mainPref.deleteProbe(p_handle);
    main << "This will be written to file";
    
    // There is also a VAR macro set to expand the value of variables
    int x = 0x1a2;
    std::string test = "This is a test of the emergency broadcast system".
    main << VAR(x); # expands out to "x: 418"
    main << VAR(test); # expands out to "test: This is a test of the emergency broadcast system"
    
    // Later if we wish to pull this already setup logger we can recreate the reference, or if you prefer use the pointer notation
    // Log::Logger& main = Log::getLog("main");
    // Log::Logger* main = Log::getLog_p("main");    
    
    return 0;
})
```

example output
```
console:
   This will be written to stream
   This will be written to stream, AND file

logging/output.log:
   This will be written to stream, AND file
   This will be written to file
   x: 418
   test: This is a test of the emergency broadcast system
```

## Levels
```c++
#include "log.h"

int main(..)
{
    // Set up a basic stdout logger
    Log::Logger& main = Log::getLog("main");
    Log::Behaviour& mainPref = main.getBehaviour();
    mainPref.addProbe(new Log::Wrapper::StdOut());

    // but in this case we're going to set a minimum listen level
    // By default it is set to Info
    mainPref.setLevel(Log::Level::Error);

    // Now anything we write that isn't at Error or above
    // will be filtered out from presentation.
    // The order goes {debug, info, warning, error, critical}
    main.write("This will be presented to screen", Log::Level::Error);

    // It's worth noting the default level is Log::Level::Info
    main << "This won't be presented to screen, it will be filtered out";

    // The listen level is set at the logger level, so any probes attached
    // will have the same level
    mainPref.addProbe(new Log::Wrapper::FileOut("log.txt"));
    main.write("This will be presented to screen, and written to file", Log::Level::Error);
    main.write("This still wont be presented to screen or file as level is info");
}
```
example output
```
stdout:
    This will be presented to screen
    This will be presented to screen, and written to file

log.txt:
    This will be presented to screen, and written to file
```

## Aggregates
```c++
#include "log.h"

int main(..)
{
    // Setup our stdout again
    // This stdout only prints warning messages and above
    Log::Logger& main = Log::getLog("main");
    Log::Behaviour& mainPref = main.getBehaviour();
    mainPref.addProbe(new Log::Wrapper::StdOut());
    mainPref.setLevel(Log::Level::Warning);

    // but this time we're going to set up another logger
    // This one listens for anything debug and up (everything)
    Log::Logger& mainDebug = Log::getLog("mainDebug");
    Log::Behaviour& mainDebugPref = mainDebug.getBehaviour();
    mainDebugPref.addProbe(new Log::Wrapper::FileOut("Log.txt"));
    mainDebugPref.setLevel(Log::Level::Debug);

    // We can create a local aggreage that takes the loggers as a list
    Log::Aggregate logs( { &main, &mainDebug } );
    
    // or we can request an aggregate from the log core and get a global
    // aggregate we can pull up from anywhere.
    Log::Aggregate& globalAggregate = Log::getAggregate( "globalAggregate", { &main, &mainDebug } );
    
    // Later if we want to pull it back up all we need to do is request the name alone (also if you prefer you can use pointer notation)
    // Log::Aggregate& globalAggregate = Log::getAggregate( "globalAggregate" );
    // Log::Aggregate* globalAggregate = Log::getAggregate_p( "globalAggregate" );
    

    // We can use the aggregate object to propegate our log output
    // to multiple locations at once. The logs will still
    // use their listen levels accordingly.
    logs.write("This will show up in file only", Log::Level::Info);
    logs.write("However, this will show up on both", Log::Level::Error);

    // The primary difference between this, and probes
    // is this will allow us to have different listen levels
    // between logs. One can be set to debug text, while the other is only
    // error messages.
    
    // We can also attach newly created Loggers to an aggregate with
    // the add log function
    Log::Logger& coolDude = Log::getLog("coolDude");
    logs.addLog(coolDude);
}
```

## Formatting

```c++
#include "log.h"

int main(..)
{
    // Setup our stdout again
    Log::Logger& main = Log::getLog("main");
    Log::Behaviour& mainPref = main.getBehaviour();
    mainPref.addProbe(new Log::Wrapper::StdOut());    
    mainPref.setLevel(Log::Level::Warning);

    // We can also set formatting strings
    // This will allow us to arrange how we like
    // There are a few predefined formatting options
    // all of them are surrounded by $().
    //
    // y m d H M S     - Year/Month/Day/Hour/Min/Second
    // 12H p           - 12 hour format/period(am/pm)
    // dn mn           - day name/month day
    // message         - The actual message to print
    // level           - level of the message being broadcasted
    // name            - name of the logger
    // 0xpid pid       - hex process ID/decimal process ID
    // 0xthread thread - hex thread ID/thread ID
    mainPref.setFormatString("$(level) ($(12H):$(M):$(S)$(p)) - $(name)[$(pid)]: $(message)"); 

    // Set up a second logger
    Log::Logger& mainDebug = Log::getLog("mainDebug");
    Log::Behaviour& mainDebugPref = main.getBehaviour();
    mainDebugPref.addProbe(new Log::Wrapper::StdOut());
    mainDebugPref.setLevel(Log::Level::Debug);
    mainDebugPref.setFormatString("$(level) - $(message)");

    // lets create a local aggregate
    Log::Aggregate logs({&main, &mainDebug});

    // Now when we print the message, the first one only gets caught by the
    // debugging probe with basic formatting
    logs.write("This will be presented to screen basic");
    ...
    // The second message gets caught by both logs
    // and formatted into both styles
    logs.write("This will be presented a bit later", Log::Level::Error);
}
```
### example output
```
    Info - This will be presented to screen basic
    ...
    Warning - This will be presented a bit later
    Warning 2:25:43pm - main[1605]: This will be presented a bit later
```

## Custom Probes
```c++
#include "log.h"
#include <iostream>

// For this example I'm just going to show an example std::cout wrapper
class MyOutputWrapper : public Log::OutputWrapper
{
public:
    // Output wrapper is an abstract class, the only method not defined
    // is the write function. All you need to do is define what the function
    // does when you "write" to it. This could be network, disk, whatever.
    void write(std::string_view output) noexcept override
    {
        std::cout << output;
    }
}
```
