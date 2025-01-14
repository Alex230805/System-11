#### NOTE: development branch on a specific architecture

# Address space for System11 os 

Hex address: 
    
    0000 - 00ff: zero page ( kernel reserved )
    0100 - 01ff: stack 
    0200 - 02ff: keyboard buffer
    
    0300 - 030f: via 1 
    0310 - 031f: via 2 

    0320: extended memory bus for ROM ( os rom and others )

        ROM A
        0320: memory register a1
        0321: memory register b1
        0322: memory register c1
        0323: memory register d1
        0324: memory data port 1 [d1,c1,b1,a1]
    
        ROM B
        0325: memory register a2
        0326: memory register b2
        0327: memory register c2
        0328: memory register d2
        0329: memory data port 2 [d2,c2,b2,a2]
    
    032a: GPIO register
    
        032a: GPIO 
        032b: GPIO 
        032c: GPIO
        032d: GPIO
    
    
    032e: memory bank decode register 
    032f: rt clock 
    
    0330 - 03ff: unused
    
    0400 - 5fff: os vram ( ~22kb -> where the os will be copied )
    
    6000 - dfff: high ram ( ~32kb -> directly accessible and banked up to 10 page or 200kb, this is the fastest ) 
    
    e000-fff9: bios ROM ( 4 kb )
    
    fffa-fffb: nm interrupt ( used by the kernel nmi sequence )
    fffc-fffd: reset/startup vector ( jump into bios )
    fffe-ffff: standard interrupt ( keyboard and other major event )
    


