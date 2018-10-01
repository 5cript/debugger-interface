# gdb-interface
A GDB mi adapter, that does rudamentary parsing

## Dependencies
cmake
tiny-process-library https://gitlab.com/eidheim/tiny-process-library/

## Usage Example
```C++
#include <gdb-interface/gdb.hpp>
#include <gdb-interface/gdb_interface.hpp>
// for just the run argument struct
// #include <gdb-interface/gdb_run_arguments.hpp>

// if you want to use the data
// #include <gdb-interface/input/stream_record.hpp>
// #include <gdb-interface/input/async_record.hpp>

#include <iostream>
#include <string>

struct MyListener : public GdbInterface::PickyListener
{
    void onRawData(std::string const& raw) override
    {
        std::cout << raw << "\n";
    }
	// more... see Listeners
};

int main()
{
    using namespace GdbInterface;

    Gdb debugger({
        "D:/msys2/mingw64/bin/gdb.exe",
        "D:/Development/IDE/debuggable/bin/Debug/debuggable.exe",
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        false,
        false,
        false,
        false,
        true
    });
    MyListener listener;
    debugger.registerListener(&listener);
    debugger.start();
    debugger.sendCommand({"-exec-run"});

    std::cout << "exit: " << debugger.waitForProcess() << "\n";
}
    
```