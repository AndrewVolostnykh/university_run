Волостних А.А. - Варiант 3

### Граматика:
```
1. <signal-program> --> <program>
2. <program> --> PROGRAM <procedure-identifier> ;
   <block>.
3. <block> --> <declarations> BEGIN <statements-list> END
4. <declarations> --> <constant-declarations>
5. <constant-declarations> --> CONST <constant-declarations-list> | <empty>
6. <constant-declarations-list> --> <constant-declaration> <constant-declarations-list> | <empty>
7. <constant-declaration> --> <constant-identifier> = <constant>;
8. <statements-list> --> <statement> <statements-list> | <empty>
9. <statement> --> <variable-identifier> := <constant> ;
10. <constant> --> <unsigned-integer>
11. <constant> --> - <unsigned-integer>
12. <constant-identifier> --> <identifier>
13. <variable-identifier> --> <identifier>
14. <procedure-identifier> --> <identifier>
15. <identifier> --> <letter><string>
16. <string> --> <letter><string> | <digit><string> | <empty>
17. <unsigned-integer> --> <digit><digits-string>
18. <digits-string> --> <digit><digits-string> | <empty>
19. <digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
20. <letter> --> A | B | C | D | ... | Z
```

### Машина кнута:
|   | command address               | command code                              	| AT	| AF	|
|===|===============================|===============================================|=======|=======|
|1  |\<signal-program\>				|\<program\>									|F		|		|
|2  |\<program\>					| PROGRAM										|		|F		|
|3  |								|\<procedure-identifier\>						|		|F		|
|4  |								|;												|		|F		|
|5  |								|\<block\>										|		|F		|
|6  |								|.												|T		|F		|
|7  |\<block\>  					|\<declarations\>								|		|8		|
|8  |								|BEGIN											|		|F		|
|9  |								|\<statements-list\>							|		|10		|
|10 |								|END											|T		|F		|
|11 |\<declarations\>				|\<constant-declarations\>						|		|F		|
|12 |\<constant-declarations\>		|CONST											|		|F		|
|13 |								|\<constant-declarations-list\>					|		|F		|
|14 |								|\<empty\>										|if(++token==BEGIN)\|<br>goto 9 else return F|
|15 |\<constant-declarations-list\>	|\<constant-declaration\>						|		|F		|
|16 |								|\<constant-declarations-list\>					|		|F		|
|17 |								|\<empty\>										|if(++token==BEGIN)\|<br>goto 9 else return F|
|18 |\<constant-declaration\>		|\<constant-identifier\>						|		|F		|
|8  |								|=												|		|F		|
|19 |								|\<constant\>									|		|F		|
|20 |								|;												|T		|F		|
|21 |\<statements-list\>			|\<statement\>									|		|F		|
|22 |								|\<statement-list\>								|		|F		|
|23 |								|\<empty\>										|if(++token==END)\|<br>return T else return F|
|24 |\<statement\>					|\<variable-identifier\>						|		|F		|
|25 |								|:=												|		|F		|
|26 |								|\<constant\>									|		|F		|
|27 |								|;												|T		|F		|
|28 |\<variable-identifier\>        |\<identifier\>                                 |T		|F		|
|29 |\<constant-identifier\>		|\<identifier\>									|T		|F		|
|30 |\<constant\>					|- \<unsigned-integer\> | \<unsigned-integer\>	|T		|F		|
|===================================================================================================|
