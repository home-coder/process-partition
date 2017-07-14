# process-partition
1.读取分区，使用读取proc方式，解析字段
2.使用读取设备路径的方式


步骤：
首先不要去解析/proc/parttions，尝试解析/proc/commondline
解析出每一个字段的信息保存到属性中

其次编译进系统，要能够在/system/bin中看到编译文件

最后去做上面的任务1和2

3.使用读取文件的方式来解析磁盘空间, 类似df命令


完成的过程：
将内存/proc/parttions数据读入buffer缓存，然后将该缓存设置为const类型，解析字段sda sdb，并提取这个字段首指针之前的那个字段947xxxx就是磁盘容量了
