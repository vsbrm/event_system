
#include "event_system.h"


namespace ev_sys
{


    event_system::event_system(int a_flags)
    {
        if ((epoll_fd = epoll_create1(a_flags)) < 0)
            throw event_sys_epoll_error(errno);
    }

    event_system::~event_system()
    {
        close(epoll_fd);
    }

    void event_system::add_event(event_handler *a_handler)
    {
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, a_handler->event_fd, &a_handler->epoll_ev))
            throw event_sys_epoll_error(errno);
        epoll_event ev = {0};
        events.push_back(ev);
    }

    void event_system::remove_event(event_handler *a_handler)
    {
        if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, a_handler->epoll_ev.data.fd, &a_handler->epoll_ev))
            throw event_sys_epoll_error(errno);
        events.erase(events.end() - 1);
    }

    void event_system::change_event(event_handler *a_handler)
    {
        if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, a_handler->epoll_ev.data.fd, &a_handler->epoll_ev))
            throw event_sys_epoll_error(errno);
    }

    int event_system::run(int a_timeout, int a_max_events)
    {
        int nfds = epoll_wait(epoll_fd, events.data(),
                (a_max_events == -1) ? events.size() : a_max_events, a_timeout);
        if (nfds < 0)
            throw event_sys_epoll_error(errno);
        for (auto i = 0; i < nfds; i++)
            ((ev_sys::event_handler *) events.at(i).data.ptr)->handle();
        return nfds;
    }


}
