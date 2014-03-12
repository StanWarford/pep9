;File: fig0601.pep
;Computer Systems, Fourth Edition
;Figure 6.1
;
         LDA     'B',i       ;push B
         STBYTEA -1,s        
         LDA     'M',i       ;push M
         STBYTEA -2,s        
         LDA     'W',i       ;push W
         STBYTEA -3,s        
         LDA     335,i       ;push 335
         STA     -5,s        
         LDA     'i',i       ;push i
         STBYTEA -6,s        
         SUBSP   6,i         ;6 bytes on the run-time stack
         CHARO   5,s         ;output B
         CHARO   4,s         ;output M
         CHARO   3,s         ;output W
         DECO    1,s         ;output 335
         CHARO   0,s         ;output i
         ADDSP   6,i         ;deallocate stack storage
         STOP                
         .END                  