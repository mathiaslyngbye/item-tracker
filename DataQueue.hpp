#ifndef _DATAQUEUE_HPP_
#define _DATAQUEUE_HPP_

#include <opencv2/opencv.hpp>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

template <class T>
class DataQueue{
    private:
        bool newdata;
        std::condition_variable cond_newdata;
        std::mutex mux_data;
        std::queue<T> data;
    public:
        T getData()
        {
            //std::scoped_lock lock(mux_data);
            std::unique_lock lock(mux_data);
            while (data.size()<1) 
                cond_newdata.wait(lock);
            T returndata;
                
            returndata=data.front();
            data.pop();
            return (returndata);
        }

        T getLastData()
        {
            //std::scoped_lock lock(mux_data);
            std::unique_lock lock(mux_data);
            while (data.size()<1) 
                cond_newdata.wait(lock);
            T returndata;
            returndata=data.back();
            return (returndata);
        }

        void addData(T in, int maxLen=3)
        {
            std::scoped_lock lock(mux_data);
            if (data.size()>=maxLen)
                data.pop();
            data.push(in);
            newdata=true;
            cond_newdata.notify_one();
        }

        void shutDown()
        {
            cond_newdata.notify_all();
        }
};

#endif
