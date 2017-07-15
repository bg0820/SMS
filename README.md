# SMS - Server Monitoring Service

### 현재 이 프로젝트는 개발중 입니다.
### (this project is not a finished project.)
### Server Monitoring Service
#### > SMS Agent
#### > Data Server
#### > Web Server (Visualization)
### 

---

## List Collected by Agent
#### **Cpu**
>###### CpuUsage
>###### CpuIdle
>###### CpuModel
#### **Memory**
>##### MemoryLoadPercent
>##### MemoryUsedByte
>##### MemoryFreeByte
>##### MemoryTotalByte
#### **System**
>##### UserName
>##### ComputerName
>##### OsVersionName
#### **Disk**
>##### Path
>##### Name
>##### FileFormat
>##### UsedBytes
>##### TotalBytes
>##### FreeBytes
>##### Type
>##### DiskPerformance
>##### ReadSec
>##### WriteSec
>##### DiskCount
#### **Process**
>##### PID
>##### CommandLine
>##### Name
>##### Path
>##### Owner
>##### CreateTime
>##### HandleCount
>##### ThreadCount
>##### MemoryUsage
>##### CpuUsage
>##### TotalThreadCount
#### **Network Connection Info**
>##### ProtocolType
>##### LocalEndpoint
>>##### IPV4
>>##### IPV6
>>##### Port
>##### RemoteEndpoint
>>##### IPV4
>>##### IPV6
>>##### Port
>##### State
>##### ProcessID
>##### CreateTime
>##### OwnerInfo
>##### NetworkConnectionCount
#### **Network Adapter Info**
>##### AdapterCount

## Network Protocol

#### SMS Agent -> (TCP)
#### (TCP) Data Server -> (WebSocket)
#### (WebSocket) Web Server



##### Cross-platform support be expected. Currently, only Windows is supported.

## Info
[![npm](https://img.shields.io/badge/email-bg0820%40naver.com-red.svg)]()
