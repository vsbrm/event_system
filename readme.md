# Event system

Simple system to handle events driven by epoll.

## Usage

### Create

```c++
ev_sys::event_system *es;
try
{
    es = new ev_sys::event_system();
}
catch (ev_sys::event_sys_epoll_error err)
{
    //deal with it
}
```

### Define handler

```c++
class my_handler : public ev_sys::event_handler
{
public:
    my_handler(int a_fd) : event_handler(a_fd) {};
    void handle () {std::cout << "handle!!!" << '\n';}
};
```

### Add handler

```c++
my_handler my_h(file_descriptor);
my_h.epoll_ev.events = EPOLLIN | EPOLLHUP;    // set some flags
es -> add_event(&my_h);                       // add handler to system
                                            // file_descriptor automaticly adds to epoll
```

### Wait and handle all events

```c++
int count_of_handled_events = es->run(max_events_per_run);              // by defaul max_events = count of added handlers
```
