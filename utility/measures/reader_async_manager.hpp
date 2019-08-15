#ifndef READER_ASYNC_MANAGER_HPP
#define READER_ASYNC_MANAGER_HPP

#include <functional>
#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
#include <vector>
#include <unordered_map>

using ReadCallback =  std::function<void(const char*, size_t, int)>;

class AsyncReaderManager
{
private:
    struct ReadTask 
    {
        void *listener;
        std::string fileName;
        ReadCallback callback;
        char *buffer;
        size_t size;
        
        explicit ReadTask(void *_listener, const std::string &_fileName, ReadCallback _callback) 
        :   listener(_listener),
            fileName(_fileName),
            callback(_callback)
            buffer(nullptr),
            size(0)
        {
        }
    };
    
    ~ReadTask() {
        if(buffer != nullptr) {
            delete[] buffer;
        }
    }
    
private:
    void readAsync(ReadTask* pTask){
			std::thread t([this](ReadTask* data)->void{
				//open file
				ifstream fin;
				fin.open(data->filename, ios::binary);
				if(fin){
					//get file size
					fin.seekg(0,ios::end);
					data->sz = fin.tellg();
					fin.seekg(0,ios::beg);
					if(data->sz > 0){
						//create buffer & read binary format
						data->buffer = new char[data->sz];
						fin.read(data->buffer,data->sz);
					}
					//close file
					fin.close();
				}
				
				//call notify of the manager, threads will be sync here
				//data will be disposed when Notify is called
				this->notify(data);	
				
			},pTask);
		}
    
public:
    int registerReadTask(void *listener, const std::string &fileName, ReadCallback callback) {
    
        if(!listener || !callback || fileName.size() <= 0) return -1;
        
        std::lock_guard<std::mutex> lock(mtx);
        
        auto task = new ReadTask(listener, fileName, callback);
        int ret = 0;
        auto iter = taskList.find(listener);
        
        if(iter != taskList.end()) {
            (iter->second).push_back(task);
            ret = (iter->second).size() - 1;
        } else {
            auto pr = taskList.insert(std::make_pair(listener, std::vector<ReadTask>()));
            (pr.second)->push_back(task);
        }
        
        readAsync(task);
        
        return ret;
    }
    
    bool widthDrawListener(void *listener, int) {
    
    }
    
private:
    std::unordered_map<void *, std::vector<ReadTask *>> taskList;
    std::mutex mtx;
};

#endif
