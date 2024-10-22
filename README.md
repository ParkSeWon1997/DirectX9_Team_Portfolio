쥬신5개월차SR프로젝트
======================

## 1. Client 프로젝트 설정
### 1.1. Property
* General
  - output directory
    + ../Bin\
* VC++ Directory
  - Include Directories
    + $(DXSDK_DIR)Include
  - Library Directories
    + $(DXSDK_DIR)lib\x86
4. 입력 :시스템라이브러리




### 1.2. C/C++
* General
  - Additonal Include Directries
    + ../Header
    + ../../Base
    + ../../Reference/Header
  
### 1.3. Linker
* General
  - Additional Library Directories
    +../../Reference/Library
* Input
  - Additional Dependencies(추가종속성)
    + d3d9.lib
    + d3dx9.lib
    + dxguid.lib
    + System.lib
    + Utility.lib
    + dinpu8.lib
      
## 2. System(dll),Utillity 프로젝트 설정
### 2.1. Property
* General
  - output directory
    + ../Bin\
* VC++ Directory
  - Include Directories
    + $(DXSDK_DIR)Include
  - Library Directories
    + $(DXSDK_DIR)lib\x86
4. 입력 :시스템라이브러리


### 2.2. C/C++
* General
  - Additonal Include Directries
    + ../../Header
    + ../../../Base
      
### 2.3. Linker
* General
  - Additional Library Directories
    + ../../Reference/Library
* Input
  - Additional Dependencies(추가종속성)
    + d3d9.lib
    + d3dx9.lib
    + dxguid.lib   
    + **System.lib** (유틸리티 프로젝트만 system.lib 추가)

    
함수
----
* ProtoMgr
  - 컴포넌트 (리소스매니저와같음)
  - 
