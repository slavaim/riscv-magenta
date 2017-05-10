/*
The code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/
#pragma once

#define PAGE_SHIFT	(12)
#define PAGE_SIZE_SHIFT PAGE_SHIFT
#define PAGE_SIZE	(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE - 1))