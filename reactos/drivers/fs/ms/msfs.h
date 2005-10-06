#ifndef __SERVICES_FS_MS_MSFS_H
#define __SERVICES_FS_MS_MSFS_H

#include <ntifs.h>
#include <ndk/ntndk.h>

/* 
 * FIXME: GCC doesn't have a working option for defaulting to a calling
 * convention. It will always default to cdecl. The MS DDK was designed
 * for compilers which support this option, and thus some of their headers
 * do not specify STDCALL or NTAPI everywhere. As such, callbacks will be
 * interpreted as cdecl on gcc, while they should be stdcall. Defining
 * NTAPI manually won't work either, since msvc will realize that the
 * two definitions are different. So we have to use something to close
 * the compatibility gap, until someone fixes gcc.
 */
#ifdef _MSC_VER
#define DEFAULTAPI
#else
#define DEFAULTAPI __stdcall
#endif

typedef struct _MSFS_DEVICE_EXTENSION
{
   LIST_ENTRY MailslotListHead;
   KMUTEX MailslotListLock;
} MSFS_DEVICE_EXTENSION, *PMSFS_DEVICE_EXTENSION;

typedef struct _MSFS_MAILSLOT
{
   UNICODE_STRING Name;
   LIST_ENTRY MailslotListEntry;
   KSPIN_LOCK FcbListLock;
   LIST_ENTRY FcbListHead;
   struct _MSFS_FCB *ServerFcb;
   ULONG ReferenceCount;
   LARGE_INTEGER TimeOut;
   KEVENT MessageEvent;
   ULONG MaxMessageSize;
   ULONG MessageCount;
   KSPIN_LOCK MessageListLock;
   LIST_ENTRY MessageListHead;
} MSFS_MAILSLOT, *PMSFS_MAILSLOT;

typedef struct _MSFS_FCB
{
   LIST_ENTRY FcbListEntry;
   PMSFS_MAILSLOT Mailslot;
} MSFS_FCB, *PMSFS_FCB;

typedef struct _MSFS_MESSAGE
{
   LIST_ENTRY MessageListEntry;
   ULONG Size;
   UCHAR Buffer[1];
} MSFS_MESSAGE, *PMSFS_MESSAGE;


#define KeLockMutex(x) KeWaitForSingleObject(x, \
                                             UserRequest, \
                                             KernelMode, \
                                             FALSE, \
                                             NULL);

#define KeUnlockMutex(x) KeReleaseMutex(x, FALSE);

NTSTATUS DEFAULTAPI MsfsCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS DEFAULTAPI MsfsCreateMailslot(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS DEFAULTAPI MsfsClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS DEFAULTAPI MsfsQueryInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS DEFAULTAPI MsfsSetInformation(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS DEFAULTAPI MsfsRead(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS DEFAULTAPI MsfsWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS DEFAULTAPI MsfsFileSystemControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS NTAPI
DriverEntry(PDRIVER_OBJECT DriverObject,
	    PUNICODE_STRING RegistryPath);

#endif /* __SERVICES_FS_NP_NPFS_H */
