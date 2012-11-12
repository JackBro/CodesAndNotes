//-----------------------------------------------------------------------------------//
//              Windows Graphics Programming: Win32 GDI and DirectDraw               //
//                             ISBN  0-13-086985-6                                   //
//                                                                                   //
//  Written            by  Yuan, Feng                             www.fengyuan.com   //
//  Copyright (c) 2000 by  Hewlett-Packard Company                www.hp.com         //
//  Published          by  Prentice Hall PTR, Prentice-Hall, Inc. www.phptr.com      //
//                                                                                   //
//  FileName   : proxy.cpp				                                             //
//  Description: API hacking gluing routines                                         //
//  Version    : 1.00.000, May 31, 2000                                              //
//-----------------------------------------------------------------------------------//

#define NOCRYPT

#include <windows.h>
#include <assert.h>

#include "..\..\include\Decoder.h"
#include "..\..\include\atom.h"

#include "Function.h"
#include "Stack.h"

// para n
// para n-1
// ...
// para 1
// return address
// func_id
// ecx = this

unsigned           OverHead = 0;
long               Lock     = 0;

void ProxyInit(void)
{
    __asm   _emit   0x0F                // read Pentium clock cycle into EDX:EAX
    __asm   _emit   0x31
    __asm   shrd    eax, edx, 8         // EAX = EDX:EAX >> 8
    __asm   mov     OverHead, eax

    Lock = 0;
}


void _stdcall ProxyExit(ExitInfo *info, unsigned leavetime)
{
    int depth;

    assert(pStack);

    KRoutineInfo * routine = pStack->Lookup(depth);

	if ( routine )
    {
        pFuncTable->FuncExitCallBack(routine, info, leavetime, depth);
        info->m_rslt = routine->rtnaddr;

        pStack->Pop();
	}
}


_declspec(naked) void ProxyEpilog(void)
{
    // placehold for return address
	__asm	push	eax					// placehold for return address

    // save common registers
    __asm   push    eax
    __asm   push    ebx
    __asm   push    ecx
    __asm   push    edx                 // 4*4=16
	__asm   pushfd						// 4
	
    // read cpu clock cycles
    __asm   _emit   0x0F                // read Pentium clock cycle into EDX:EAX
    __asm   _emit   0x31				// time3
    __asm   shrd    eax, edx, 8         // EAX = EDX:EAX >> 8

    // push leaving clock, leaving time on stack
    __asm   push    eax                 // leaving clock
    __asm   sub     eax, OverHead       
    __asm   push    eax                 // leaving clock - overhead

  //  while (Lock);                       // wait for lock
  //  InterlockedIncrement(& Lock);

    // pushing pointer to return address placehold, call ProxyExit
    __asm	lea		eax, [esp+28]		// offset for eax(placehold) on stack
	__asm	push	eax
	__asm	call	ProxyExit

    // read cpu clock cycle, update Overhead
    __asm   pop     ecx                 // ecx = entering clock
    __asm   _emit   0x0F				// time4
    __asm   _emit   0x31
    __asm   shrd    eax, edx, 8         // EAX = EDX:EAX >> 8
    __asm   sub     eax, ecx            // new overhead by ProxyEpilog
    __asm   add     OverHead, eax
    
 // InterlockedDecrement(& Lock);

    // restore common registers
    __asm   popfd
    __asm   pop     edx
    __asm   pop     ecx
    __asm   pop     ebx
    __asm   pop     eax

    // return to address stored in placehold, the original caller
	__asm	ret
}

void _stdcall ProxyEntry(EntryInfo *info, unsigned entertime)
{
	int id = info->m_funcid;
	
    assert(pStack!=NULL);
    KRoutineInfo * routine = pStack->Push();

	if ( routine )
    {
        routine->entertime = entertime;
        routine->funcid    = id;
        routine->rtnaddr   = info->m_rtnads;

        pFuncTable->FuncEntryCallBack(routine, info);
    
        // if pushing is succeful, patch return address such that original 
        // function will go to our epilog code before returning to caller
		info->m_rtnads = (unsigned) ProxyEpilog;
    }            
    
    // make sure controls goes to original function when ProxyProlog returns
	info->m_funcid = (unsigned) pFuncTable->m_func[id].f_oldaddress;
}



//   ProxyProlog        inline assembly naked
//       ProxyEntry     C++             stdcall
//   Original Callee
//   ProxyEpilog        inline assembly naked
//       ProxyExit      C++             stdcall
//   Original caller    
_declspec(naked) void ProxyProlog(void)
{
                                        // funcid already pushed on stack by stub code 
                                        // generated by KFunction::InitStub
    // funcid already on stack, place hold for original callee address

    // save common registers
    __asm   push    eax
    __asm   push    ebx
	__asm	push	ecx					
    __asm   push    edx                 // edx, ecx, ebx, eax, funcid, rtnadr, p1..pn
	__asm   pushfd						// 4      bytes EFLAGS
	
    // read cpu clock cycle count
    __asm   _emit   0x0F                // read Pentium clock cycle into EDX:EAX
    __asm   _emit   0x31				// time1
    __asm   shrd    eax, edx, 8         // EAX = EDX:EAX >> 8

    // pushing entering clock, entering time
    __asm   push    eax                 // entering clock
    __asm   sub     eax, OverHead       
    __asm   push    eax                 // entering clock - overhead

 // while (Lock);
 // InterlockedIncrement(& Lock);

    // push offset to common registers, call ProxyEntry
	__asm	lea		eax, [esp+8]		// offset for flag on stack		
	__asm	push	eax
	__asm	call    ProxyEntry
	
    // read cpu clock cycle count, update overhead
    __asm   pop     ecx                 // ecx = entering clock
    __asm   _emit   0x0F				// time2
    __asm   _emit   0x31
    __asm   shrd    eax, edx, 8         // EAX = EDX:EAX >> 8
    __asm   sub     eax, ecx            // new overhead by ProxyEntry
    __asm   add     OverHead, eax

 // InterlockedDecrement(& Lock);

    // restore common registers
	__asm	popfd
    __asm   pop     edx
	__asm	pop		ecx					
    __asm   pop     ebx
    __asm   pop     eax

    // switch control to original callee
	__asm	ret                         // control continues to original callee
}
