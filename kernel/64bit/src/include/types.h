#ifndef __KERNEL_TYPES_H__
#define __KERNEL_TYPES_H__

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;
typedef unsigned long	u64;

typedef	char	i8;
typedef short	i16;
typedef int	i32;
typedef long	i64;

typedef float	f32;
typedef double	f64;

#define NULL ((void*)0)

#define __NORETURN__ __attribute__((noreturn))
#define __PACKED__ __attribute__((packed))
#define __ALIGNED__(x) __attribute__((aligned(x)))

#endif /* __KERNEL_TYPES_H__ */
