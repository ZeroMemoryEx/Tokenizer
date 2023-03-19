# Tokenizer

* Tokenizer is a kernel mode driver project that allows the replacement of a process token in `EPROCESS` with a system token, effectively elevating the privileges of the process, The driver is designed to be used with a user-mode application that sends a process ID to the driver through an `IOCTL`.

# technical details

* When a process is created, it inherits the token of the user who created it, The token is used by the system to determine what actions the process can perform. The token contains information about the user's security identifier (SID), group memberships, and privileges.

  ![image](https://user-images.githubusercontent.com/60795188/226148214-1d63149a-e2e6-4938-9067-30df7939c9db.png)
  
* The Token member resides at offset `0x4b8` in the `_EPROCESS` structure, which is a data structure that represents a process object. The Token member is defined in  `_EX_FAST_REF` structure, which is a union type that can store either a pointer to a kernel object or a reference count, depending on the size of the pointer , The offset of the `_EX_FAST_REF` structure within `_EPROCESS` depends on the specific version of Windows being used, but it is typically located at an offset of `0x4b8` in recent versions of Windows..

* Windows Build Number token Offsets for x64 and x86 Architectures

  | x64 offsets    | x86 offsets        |
  | --------------| ------------------ |
  | 0x0160 (late 5.2) | 0x0150 (3.10)      |
  | 0x0168 (6.0)  | 0x0108 (3.50 to 4.0) |
  | 0x0208 (6.1)  | 0x012C (5.0)        |
  | 0x0348 (6.2 to 6.3) | 0xC8 (5.1 to early 5.2) |
  | 0x0358 (10.0 to 1809) | 0xD8 (late 5.2) |
  | 0x0360 (1903) | 0xE0 (6.0)          |
  | 0x04B8        | 0xF8 (6.1)          |
  |               | 0xEC (6.2 to 6.3)   |
  |               | 0xF4 (10.0 to 1607) |
  |               | 0xFC (1703 to 1903) |
  |               | 0x012C              |


    ![image](https://user-images.githubusercontent.com/60795188/226148257-b679202e-2371-4bda-98ea-689107221075.png)
  
* The `_EX_FAST_REF` structure in Windows contains three members: `Object` and `RefCount` and `Value`

  ![image](https://user-images.githubusercontent.com/60795188/226148720-8807b491-591c-479c-981f-734c1e868981.png)

* To display the process token in `_EX_FAST_REF`,We pass the address of the `_EX_FAST_REF` structure that contains the token, which is typically located at an offset of `0x4b8` in the `_EPROCESS` structure."

  ![image](https://user-images.githubusercontent.com/60795188/226148478-4e0c6c05-7a4c-4214-b484-0cdd8fc1c2e8.png)
  
# Usage

* Note: In the next update, i will add a feature to spawn a privileged Command Prompt process .

* First, you need to either spawn a new process or choose an already existing process ID, For the sake of this explanation, i will use a cmd as an example. 

  ![image](https://user-images.githubusercontent.com/60795188/226149275-cfd76437-dda3-4964-9a54-43fa20247b3e.png)
  
* inherited Token

  ![image](https://user-images.githubusercontent.com/60795188/226149373-2bf16ae9-e67f-4150-86b3-8376b0eb8428.png)
  
* send the Process ID to the driver through an IOCTL 

  ![image](https://user-images.githubusercontent.com/60795188/226196873-f5cd9ab4-5c71-4d05-a0d4-4ae80a8dd809.png)


* After receiving the PID from the user mode application, the driver uses it to obtain a pointer to the `_EPROCESS` structure for the target process. The driver then accesses the Token member of the `_EPROCESS` structure to obtain a pointer to the process token, which it replaces with the system token, effectively changing the security context of the process to that of the system. However, if the driver does not correctly locate the Token member within the `_EPROCESS` structure or if the offset of the Token is other than `0x4b8` , the driver may crash the system or the target process ,this problem will be fixed in the next updates .

  ![image](https://user-images.githubusercontent.com/60795188/226149604-dd0e4f82-b3fa-43a2-97c4-c37f3fb2eebf.png)
 
* cmd token after
 
  ![image](https://user-images.githubusercontent.com/60795188/226149646-a781b2d1-6590-4210-80fa-1b34a6bd680d.png)
 
* the process privileges, groups, rights 
  
  ![image](https://user-images.githubusercontent.com/60795188/226149800-e80ea9d8-5f69-4425-ad0e-a4a65cd946d9.png)

# DEMO

  https://user-images.githubusercontent.com/60795188/226200873-d0516968-b175-4ff4-8e85-02018c641679.mp4


