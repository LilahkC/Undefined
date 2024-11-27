#include <glad/glad.h>

#include "application.h"

#include "memory_leak.h"

#include "service_locator.h"

int main()
{
    Application app;

    app.Init();

    // Let the window open until we press escape or the window should close
    while (ServiceLocator::Get<Window>()->IsWindowOpen())
    {
        app.Update();
    }

    app.Clear();

    MemoryLeak::EndMemoryLeak();
    return 0;
}
