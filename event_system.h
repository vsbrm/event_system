#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H


#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <cstring> // strerror()
#include <string>
#include <vector>


namespace ev_sys
{
    class event_handler;
    class event_system
    {
        int epoll_fd;
        std::vector<epoll_event> events;
    public:
        event_system(int a_flags = 0);
        ~event_system();
        void add_event(event_handler *a_handler);
        void remove_event(event_handler *a_handler);
        void change_event(event_handler *a_handler);
        int run(int a_timeout = -1, int a_max_events = -1);
    };


    class event_handler
    {
    public:
        int event_fd;
        epoll_event epoll_ev;
        event_handler(int a_fd)
                {epoll_ev.data.ptr = this; event_fd = a_fd;};
        ~event_handler() {};
        virtual void handle() = 0;
    };

    class event_sys_epoll_error : public std::exception
    {
        int code;
    public:
        std::string error_message;
        event_sys_epoll_error(int a_errno)
        {
            code = a_errno;
            error_message = "error in epoll: ";
            error_message += strerror(code);
        };
        const char * what() const noexcept
        {
            return error_message.c_str();
        };
    };

}

#endif
