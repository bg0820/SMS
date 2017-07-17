# SMS - Server Monitoring Service


### **Server Monitoring Service (SMS)**
>#### SMS Agent
>#### Data Server
>#### Web Server (Visualization)
---
##### 현재 이 프로젝트는 개발중 입니다.
##### this project is work in progress.
##### 크로스 플랫폼 지원 예정입니다. 현재는 윈도우만 지원중
##### Cross-platform support is being planned. Currently, only Windows is supported.

## List Collected by Agent

* Cpu
  * CpuUsage
  * CpuIdle
  * CpuModel
* Memory
  * MemoryLoadPercent
  * MemoryUsedByte
  * MemoryFreeByte
  * MemoryTotalByte
* System Info
  * UserName
  * ComputerName
  * OsVersionName
* Disk
  * Path
  * Name
  * FileFormat
  * UsedBytes
  * TotalBytes
  * FreeBytes
  * Type
  * DiskPerformance
  * ReadSec
  * WriteSec
  * DiskCount
* Process
  * PID
  * CommandLine
  * Name
  * Path
  * Owner
  * CreateTime
  * HandleCount
  * ThreadCount
  * MemoryUsage
  * CpuUsage
  * TotalThreadCount
* Network Connection Info
  * ProtocolType
  * LocalEndpoint
    * IPV4
    * IPV6
    * Port
  * RemoteEndpoint
    * IPV4
    * IPV6
    * Port
  * State
  * ProcessID
  * CreateTime
  * OwnerInfo
  * NetworkConnectionCount
* Network Adapter Info
  * AdapterCount

## Network Protocol

#### SMS Agent -> (TCP)
#### (TCP) Data Server -> (WebSocket)
#### (WebSocket) Web Server

## Library
| Library Name | Link | License |
| :---------------: | :-----------------: | :-------------------: |
| RapidJson      | <https://github.com/miloyip/rapidjson/> |  |

## Commit Message
https://gitmoji.carloscuesta.me/

## Info
[![npm](https://img.shields.io/badge/email-bg0820%40naver.com-red.svg)]()
