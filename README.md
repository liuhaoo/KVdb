# KVdb
## 项目描述

项目是使用C++实现基于跳表的KV数据库，可以完成键值对的增删改查，设置过期时间，以及数据的加载落盘等操作。

在随机写读情况下，该项目每秒可处理写请求数（QPS）: 52.72w，每秒可处理读请求数（QPS）: 49.74w（测试条件基于实验室服务器下，不同性能和环境有影响）

## 主要工作
- 实现了基于跳表的存储引擎，能够进行键值对增删改查，文件加载，数据落盘等操作；
- 基于LRU算法，实现了对键值对的Cache查询；
- 实现了数据库对过期键值对进行定期删除和惰性删除的策略.
- 跳表使用泛型编程，支持存储int，string等基本数据类型；
- 使用锁机制保证读写分离，在云服务器测试下每秒支持五十万次以上随机读写

## 接口定义
- insertElement（插入数据）
- searchElement（查询数据）
- deleteElement（删除数据）
- displayList（展示已存数据）
- dumpFile（数据落盘）
- loadFile（加载数据）
- element_expire_time（设置过期时间）
- element_ttl（查询数据过期时间）
- cycle_del（定期删除）
- printLRU（展示LRU缓存）


## 性能测试
### 测试环境
分别按照 10万、50万、100万条存取请求对存储引擎进行压力测试，采用随机插入和读取数据进行测试。

SkipList树高设置为：18

硬件平台：
- 阿里云服务器（2核vCPU，2 GiB）

编译环境
- gcc version 8.4.0 
- Ubuntu  18.04 64位

### 插入数据测试
单线程写入指只有一个线程向存储引擎写入数据，双线程写入指两个线程同时向存储引擎写入数据。

| 插入数据规模/万条 | 单线程写入耗时/s | 双线程写入耗时/s |
|------------|------------|-----------|
| 10 | 0.101105 | 0.286647 |
| 50 | 0.780548 | 1.93864 |
| 100 | 1.89674 | 4.38551 |

- 单线程下，每秒可处理写请求数（QPS）：52.722w
- 双线程下，每秒可处理写请求数（QPS）：22.802w
- **注：最终QPS 取数据量最大的 QPS**

### 查询数据测试
单线程读取指只有一个线程向存储引擎读取数据，双线程读取指两个线程同时向存储引擎读取数据。


| 查询数据规模/万条 | 单线程读取耗时/s | 双线程读取耗时/s |
|------------|------------------|-----------|
| 10 | 0.0775424 | 0.0630571 |
| 50 | 0.757941 | 0.46968 |
| 100 | 2.01044 | 1.07307 |

- 单线程下，每秒可处理写请求数（QPS）：49.741w
- 双线程下，每秒可处理写请求数（QPS）：93.191w
- **注：最终QPS 取数据量最大的 QPS**




