stateDiagram-v2
    state "Stack/Heap Allocation" as ScopeStart
    state "Constructor Run" as Init
    state "Driver State: ACTIVE" as Running
    state "Destructor Run" as Deinit
    state "Memory Reclaimed" as ScopeEnd

    [*] --> ScopeStart
    ScopeStart --> Init : Instance Created
    Init --> Running : esp_wifi_init() \n esp_wifi_start()
    Running --> Running : System Operations
    Running --> Deinit : Object goes out of scope \n (RAII Trigger)
    Deinit --> ScopeEnd : esp_wifi_stop() \n esp_wifi_deinit()
    ScopeEnd --> [*]

    note right of Init : RESOURCE ACQUIRED
    note right of Deinit : RESOURCE RELEASED (Guaranteed)