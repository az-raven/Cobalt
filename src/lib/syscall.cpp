#pragma once

#include <stdint.h>

#include "irq.cpp"

// FIXME: this is extremely concerning
typedef uint16_t errno_t;

enum ERRNO {
    /// @brief Operation success.
    ESUCCESS = 0,
    /// @brief Operation not permitted
    EPERM = 1,
    /// @brief No such file or directory
    ENOENT = 2,
    /// @brief No such process
    ESRCH = 3,
    /// @brief Interrupted system call
    EINTR = 4,
    /// @brief Input/output error
    EIO = 5,
    /// @brief No such device or address
    ENXIO = 6,
    /// @brief Argument list too long
    E2BIG = 7,
    /// @brief Exec format error
    ENOEXEC = 8,
    /// @brief Bad file descriptor
    EBADF = 9,
    /// @brief No child processes
    ECHILD = 10, 
    /// @brief Resource temporarily unavailable
    EAGAIN = 11,
    /// @brief Cannot allocate memory
    ENOMEM = 12,
    /// @brief Permission denied
    EACCES = 13, 
    /// @brief Bad address
    EFAULT = 14,
    /// @brief Block device required
    ENOTBLK = 15, 
    /// @brief Device or resource busy
    EBUSY = 16,
    /// @brief File exists
    EEXIST = 17,
    /// @brief Invalid cross-device link
    EXDEV = 18,
    /// @brief No such device
    ENODEV = 19,
    /// @brief Not a directory
    ENOTDIR = 20,
    /// @brief Is a directory
    EISDIR = 21,
    /// @brief Invalid argument
    EINVAL = 22,
    /// @brief Too many open files in system
    ENFILE = 23,
    /// @brief Too many open files
    EMFILE = 24,
    /// @brief Inappropriate ioctl for device       
    ENOTTY = 25,
    /// @brief Text file busy
    ETXTBSY = 26,
    /// @brief File too large
    EFBIG = 27,
    /// @brief No space left on device
    ENOSPC = 28,
    /// @brief Illegal seek
    ESPIPE = 29,
    /// @brief Read-only file system
    EROFS = 30,
    /// @brief Too many links
    EMLINK = 31,
    /// @brief Broken pipe
    EPIPE = 32,
    /// @brief Numerical argument out of domain       
    EDOM = 33,
    /// @brief Numerical result out of range        
    ERANGE = 34,
    /// @brief Resource deadlock avoided
    EDEADLK = 35,
    /// @brief File name too long
    ENAMETOOLONG = 36,
    /// @brief No locks available
    ENOLCK = 37,
    /// @brief Function not implemented
    ENOSYS = 38,
    /// @brief Directory not empty
    ENOTEMPTY = 39,
    /// @brief Too many levels of symbolic links     
    ELOOP = 40,
    /// @brief Resource temporarily unavailable
    EWOULDBLOCK = 11,
    /// @brief No message of desired type
    ENOMSG = 42,
    /// @brief No message of desired type
    EIDRM = 43,
    /// @brief Channel number out of range
    ECHRNG = 44,
    /// @brief Level 2 not synchronized
    EL2NSYNC = 45,
    /// @brief Level 3 halted
    EL3HLT = 46,
    /// @brief Level 3 reset
    EL3RST = 47,
    /// @brief Link number out of range
    ELNRNG = 48,
    /// @brief Protocol driver not attached        
    EUNATCH = 49,
    /// @brief No CSI structure available
    ENOCSI = 50,
    /// @brief Level 2 halted
    EL2HLT = 51,
    /// @brief Invalid exchange
    EBADE = 52,
    /// @brief Invalid request descriptor
    EBADR = 53,
    /// @brief Exchange full
    EXFULL = 54,
    /// @brief No anode
    ENOANO = 55,
    /// @brief Invalid request code
    EBADRQC = 56,
    /// @brief Invalid slot
    EBADSLT = 57,
    /// @brief Resource deadlock avoided
    EDEADLOCK = 35,
    /// @brief Bad font file format
    EBFONT = 59,
    /// @brief Device not a stream
    ENOSTR = 60,
    /// @brief No data available
    ENODATA = 61,
    /// @brief Timer expired
    ETIME = 62,
    /// @brief Out of streams resources
    ENOSR = 63,
    /// @brief Machine is not on the network
    ENONET = 64,
    /// @brief Package not installed
    ENOPKG = 65,
    /// @brief Object is remote
    EREMOTE = 66,
    /// @brief Link has been severed
    ENOLINK = 67,
    /// @brief Advertise error
    EADV = 68,
    /// @brief Srmount error
    ESRMNT = 69,
    /// @brief Communication error on send
    ECOMM = 70,
    /// @brief Protocol error
    EPROTO = 71,
    /// @brief Multihop attempted
    EMULTIHOP = 72,
    /// @brief RFS specific error
    EDOTDOT = 73,
    /// @brief Bad message
    EBADMSG = 74,
    /// @brief Value too large for defined data type
    EOVERFLOW = 75,
    /// @brief Name not unique on network
    ENOTUNIQ = 76,
    /// @brief File descriptor in bad state
    EBADFD = 77,
    /// @brief Remote address changed
    EREMCHG = 78,
    /// @brief Can not access a needed shared library
    ELIBACC = 79,
    /// @brief Accessing a corrupted shared library
    ELIBBAD = 80,
    /// @brief .lib section in a.out corrupted
    ELIBSCN = 81,
    /// @brief Attempting to link in too many shared libraries
    ELIBMAX = 82,
    /// @brief Cannot exec a shared library directly
    ELIBEXEC = 83,
    /// @brief Invalid or incomplete multibyte or wide character
    EILSEQ = 84,
    /// @brief Interrupted system call should be restarted
    ERESTART = 85,
    /// @brief Streams pipe error
    ESTRPIPE = 86,
    /// @brief Too many users
    EUSERS = 87,
    /// @brief Socket operation on non-socket
    ENOTSOCK = 88,
    /// @brief Destination address required
    EDESTADDRREQ = 89,
    /// @brief Message too long
    EMSGSIZE = 90,
    /// @brief Protocol wrong type for socket
    EPROTOTYPE = 91,
    /// @brief Protocol not available
    ENOPROTOOPT = 92,
    /// @brief Protocol not supported
    EPROTONOSUPPORT = 93,
    /// @brief Socket type not supported
    ESOCKTNOSUPPORT = 94,
    /// @brief Operation not supported
    EOPNOTSUPP = 95,
    /// @brief Protocol family not supported
    EPFNOSUPPORT = 96,
    /// @brief Address family not supported by protocol
    EAFNOSUPPORT = 97,
    /// @brief Address already in use
    EADDRINUSE = 98,
    /// @brief Cannot assign requested address
    EADDRNOTAVAIL = 99,
    /// @brief Network is down
    ENETDOWN = 100,
    /// @brief Network is unreachable
    ENETUNREACH = 101,
    /// @brief Network dropped connection on reset
    ENETRESET = 102,
    /// @brief Software caused connection abort
    ECONNABORTED = 103,
    /// @brief Connection reset by peer
    ECONNRESET = 104,
    /// @brief No buffer space available
    ENOBUFS = 105,
    /// @brief Transport endpoint is already connected
    EISCONN = 106,
    /// @brief Transport endpoint is not connected
    ENOTCONN = 107,
    /// @brief Cannot send after transport endpoint shutdown
    ESHUTDOWN = 108,
    /// @brief Too many references: cannot splice
    ETOOMANYREFS = 109,
    /// @brief Connection timed out
    ETIMEDOUT = 110,
    /// @brief Connection refused
    ECONNREFUSED = 111,
    /// @brief Host is down
    EHOSTDOWN = 112,
    /// @brief No route to host
    EHOSTUNREACH = 113,
    /// @brief Operation already in progress
    EALREADY = 114,
    /// @brief Operation now in progress
    EINPROGRESS = 115,
    /// @brief Stale file handle
    ESTALE = 116,
    /// @brief Structure needs cleaning
    EUCLEAN = 117,
    /// @brief Not a XENIX named type file
    ENOTNAM = 118,
    /// @brief No XENIX semaphores available
    ENAVAIL = 119,
    /// @brief Is a named type file
    EISNAM = 120,
    /// @brief Remote I/O error
    EREMOTEIO = 121,
    /// @brief Disk quota exceeded
    EDQUOT = 122,
    /// @brief No medium found
    ENOMEDIUM = 123,
    /// @brief Wrong medium type
    EMEDIUMTYPE = 124,
    /// @brief Operation canceled
    ECANCELED = 125,
    /// @brief Required key not available
    ENOKEY = 126,
    /// @brief Key has expired
    EKEYEXPIRED = 127,
    /// @brief Key has been revoked
    EKEYREVOKED = 128,
    /// @brief Key was rejected by service
    EKEYREJECTED = 129,
    /// @brief Owner died
    EOWNERDEAD = 130,
    /// @brief State not recoverable
    ENOTRECOVERABLE = 131,
    /// @brief Operation not possible due to RF-kill
    ERFKILL = 132,
    /// @brief Memory page has hardware error
    EHWPOISON = 133,
    /// @brief Operation not supported
    ENOTSUP = 95
};

extern "C" errno_t syscall(
    uint32_t eax,
    uint32_t ebx,
    uint32_t ecx,
    uint32_t edx,
    uint32_t esi,
    uint32_t edi,
    uint32_t ebp
) {
    eoi(0x80);
    return EINVAL;
}