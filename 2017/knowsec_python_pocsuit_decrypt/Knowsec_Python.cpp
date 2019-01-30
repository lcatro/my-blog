__int64 __fastcall PyEval_EvalFrameEx(signed __int64 a1, const char *a2)
{
  signed __int64 PyFrameObject; // r12@1
  int v3; // ebx@2
  signed __int64 v4; // rcx@2
  int v5; // ebp@3
  __int64 Code; // r11@4
  sem_t *v7; // rdi@5
  __int64 v8; // ST28_8@5
  signed __int64 v9; // r8@5
  signed __int64 v10; // r9@6
  signed __int64 v11; // rbp@6
  signed __int64 next_instr; // r13@6
  __int64 *v13; // r10@7
  sem_t *v14; // rbx@8
  __int64 *v15; // r14@8
  __int64 v16; // r11@9
  signed int v17; // edx@11
  signed int v18; // eax@11
  signed int opcode; // er15@12
  signed __int64 v20; // rbp@13
  int v21; // er12@13
  __int64 v22; // rbx@17
  __int64 v23; // ST48_8@17
  __int64 *v24; // rax@17
  sem_t *v25; // rbx@24
  int v26; // eax@24
  __int64 v27; // rbp@30
  unsigned __int8 v28; // dh@33
  int v29; // er13@33
  int v30; // er12@33
  int v31; // er15@33
  int v32; // er14@33
  signed __int64 v33; // r10@33
  __int64 v34; // rbx@33
  void *v35; // rdi@33
  __int64 v36; // r8@35
  void *v37; // r12@35
  int v38; // eax@35
  __int64 v39; // rax@36
  __int64 v40; // r15@37
  __int64 v41; // r11@38
  __int64 v42; // rsi@38
  int v43; // er9@40
  __int64 v44; // rsi@41
  int v45; // er13@41
  __int64 *v46; // r11@41
  __int64 v47; // r15@42
  __int64 v48; // r8@42
  __int64 v49; // rsi@45
  __int64 *v50; // rax@45
  unsigned __int64 v51; // rbx@48
  _QWORD *v52; // r12@49
  __int64 v53; // r10@55
  __int64 v54; // ST48_8@57
  __int64 v55; // r15@58
  __int64 v56; // r13@59
  __int64 v57; // rax@60
  __int64 v58; // rax@62
  __int64 v59; // rbx@65
  signed __int64 v60; // r15@65
  __int64 v61; // r13@65
  __int64 *v62; // r10@67
  signed int v63; // er14@67
  int v64; // ebp@68
  __int64 v65; // rdx@69
  __int64 v66; // rbx@71
  __int64 v67; // rbp@74
  _QWORD *v68; // rcx@74
  __int64 *v70; // rax@80
  __int64 *v71; // rax@87
  __int64 v72; // rdx@88
  __int64 v73; // rsi@88
  _QWORD *v74; // r8@95
  _QWORD *v75; // ST90_8@97
  __int64 v76; // r10@99
  __int64 v77; // rsi@99
  __int64 v78; // r12@106
  __int64 v79; // rax@107
  __int64 v80; // r15@107
  __int64 v81; // rax@109
  int v82; // er11@111
  int v83; // er13@112
  __int64 *v84; // rax@112
  __int64 v85; // r10@113
  __int64 v86; // rdx@113
  __int64 v87; // rax@114
  int (__fastcall *v88)(__int64, __int64, __int64, signed __int64); // r13@115
  int v89; // edi@116
  __int64 *v90; // rax@117
  __int64 v91; // rbx@123
  int v92; // eax@123
  __int64 v93; // r12@129
  __int64 v94; // rbx@129
  __int64 v95; // r15@129
  signed __int64 v96; // rax@132
  __int64 *v97; // rbx@142
  _DWORD *v98; // r12@150
  signed __int64 v99; // rax@150
  signed __int64 v100; // r13@150
  __int64 v101; // r15@152
  signed __int64 v102; // rsi@156
  __int64 v103; // rax@158
  void *v104; // r10@162
  __int64 v105; // rdx@165
  _QWORD *v106; // r13@167
  int v107; // eax@168
  signed __int64 v108; // r15@169
  int v109; // er12@169
  _QWORD *v110; // rbx@169
  __int64 v111; // rsi@170
  _QWORD *v112; // r10@170
  __int64 v113; // ST48_8@173
  __int64 *v114; // r13@174
  signed __int64 v115; // r11@180
  __int64 v116; // rbx@180
  __int64 v117; // r15@180
  void *v118; // r14@180
  __int64 v119; // rax@182
  __int64 v120; // r10@182
  __int64 v121; // ST48_8@183
  __int64 *v122; // rax@183
  __int64 v123; // rax@184
  int v124; // er12@184
  __int64 v125; // rdx@187
  int (__fastcall *v126)(__int64, __int64); // rax@188
  __int64 v127; // ST48_8@189
  int v128; // eax@189
  __int64 v129; // ST48_8@200
  __int64 *v130; // rax@201
  int v131; // edx@206
  int v132; // er12@207
  __int64 *v133; // r10@207
  __int64 v134; // r13@208
  int (__fastcall *v135)(sem_t *, __int64); // r14@211
  sem_t *v136; // r15@211
  __int64 v137; // r13@214
  __int64 *v138; // rax@215
  int v139; // er10@218
  __int64 v140; // r14@219
  int (__fastcall *v141)(__int64, __int64, _QWORD); // r9@222
  __int64 *v142; // rax@223
  __int64 v143; // ST48_8@226
  __int64 v144; // ST48_8@227
  __int64 v145; // r13@230
  __int64 v146; // rbx@230
  int v147; // eax@230
  int v148; // eax@238
  int v149; // eax@239
  signed __int64 v150; // rbx@239
  signed __int64 v151; // r12@240
  signed __int64 v152; // rsi@243
  sem_t **v153; // r12@244
  int v154; // edi@249
  int v155; // er12@249
  __int64 v156; // rbx@250
  __int64 *v157; // rax@250
  int v158; // edi@254
  int v159; // er12@254
  int v160; // edx@256
  int v161; // er12@256
  signed __int64 v162; // r11@258
  __int64 v163; // rsi@260
  __int64 v164; // ST48_8@261
  __int64 *v165; // rax@261
  __int64 *v166; // rax@263
  __int64 v167; // r13@264
  int (__fastcall *v168)(__int64, __int64); // r14@267
  __int64 v169; // ST48_8@268
  __int64 *v170; // rax@268
  __int64 v171; // ST48_8@272
  __int64 v172; // rax@275
  int v173; // er13@278
  int v174; // er12@279
  _QWORD *v175; // r15@279
  __int64 v176; // rax@280
  __int64 v177; // r12@282
  __int64 v178; // rbx@284
  __int64 v179; // r15@286
  _QWORD *v180; // rsi@286
  __int64 v181; // r10@286
  __int64 v182; // rax@289
  __int64 v183; // r13@289
  __int64 v184; // r8@291
  __int64 v185; // r9@291
  __int64 v186; // rdi@293
  __int64 v187; // rax@293
  unsigned __int64 v188; // r13@294
  __int64 v189; // r10@296
  __int64 v190; // rax@296
  unsigned __int64 v191; // r14@297
  __int64 v192; // r9@298
  int (__fastcall *v193)(__int64, unsigned __int64, unsigned __int64, signed __int64, signed __int64, signed __int64); // rax@299
  __int64 v194; // ST48_8@301
  __int64 *v195; // rax@301
  __int64 v196; // ST48_8@309
  __int64 v197; // r13@311
  _QWORD *v198; // r11@311
  __int64 v199; // r12@311
  int v200; // eax@311
  __int64 v201; // ST48_8@314
  __int64 v202; // r10@315
  __int64 v203; // rbx@316
  signed __int64 v204; // r11@316
  __int64 v205; // r12@316
  __int64 v206; // r13@316
  void *v207; // rax@316
  __int64 v208; // ST48_8@318
  __int64 *v209; // rax@318
  __int64 v210; // r13@324
  __int64 v211; // rbx@324
  __int64 *v212; // rax@324
  __int64 v213; // ST48_8@327
  __int64 v214; // r15@328
  __int64 v215; // r12@330
  __int64 *v216; // rax@335
  __int64 v217; // ST48_8@338
  __int64 *v218; // rax@338
  __int64 v219; // ST48_8@351
  int v220; // eax@353
  __int64 v221; // ST48_8@270
  __int64 *v222; // rax@362
  __int64 v223; // rcx@368
  __int64 v224; // r14@368
  __int64 v225; // ST48_8@369
  __int64 *v226; // rax@369
  __int64 v227; // rbx@370
  __int64 v228; // rdx@378
  __int64 v229; // r14@380
  int (__fastcall *v230)(__int64, __int64); // r12@381
  __int64 v231; // ST48_8@382
  int v232; // eax@382
  __int64 v233; // rbx@385
  __int64 v234; // ST48_8@387
  __int64 *v235; // rax@388
  char *v236; // rdx@391
  __int64 v237; // rcx@391
  __int64 v238; // r8@391
  __int64 v239; // r15@393
  __int64 *v240; // rax@393
  __int64 v241; // r13@397
  __int64 v242; // r12@397
  __int64 v243; // rbx@397
  const char *v244; // rsi@398
  __int64 *v245; // rax@398
  __int64 v246; // r13@405
  __int64 v247; // rsi@405
  int v248; // eax@405
  __int64 *v249; // rax@409
  __int64 v250; // ST48_8@411
  __int64 *v251; // rax@412
  __int64 v252; // r8@415
  __int64 v253; // rdi@416
  __int64 v254; // rcx@416
  int (__fastcall *v255)(__int64, __int64); // rax@418
  __int64 *v256; // rax@419
  __int64 v257; // r8@422
  __int64 v258; // rcx@422
  const char *v259; // rdx@422
  int v260; // eax@426
  __int64 *v261; // rax@432
  __int64 v262; // r8@435
  char v263; // r14@436
  int v264; // er13@436
  int v265; // er15@436
  int v266; // er14@436
  signed __int64 v267; // rax@436
  signed __int64 v268; // r8@436
  sem_t *v269; // rbx@436
  sem_t *v270; // r12@439
  sem_t **v271; // r8@441
  sem_t *v272; // r15@441
  signed int v273; // er14@442
  signed __int64 v274; // rdx@444
  __int64 v275; // rdi@448
  signed __int64 v276; // ST90_8@449
  void *v277; // r10@450
  _QWORD *v278; // rax@453
  _QWORD *v279; // r8@454
  _QWORD *v280; // r11@455
  _QWORD *v281; // rax@456
  signed __int64 v282; // r10@456
  _QWORD *v283; // rdx@457
  int v284; // er11@457
  __int64 v285; // rdi@459
  void *v286; // r13@460
  signed __int64 v287; // ST90_8@461
  __int64 *v288; // rax@461
  char *v289; // r13@468
  __int64 v290; // rbx@473
  signed __int64 v291; // r11@473
  __int64 *v292; // r9@473
  __int64 *v293; // r13@474
  __int64 v294; // r15@474
  __int64 *v295; // r12@474
  __int64 v296; // rax@475
  __int64 *v297; // r10@475
  __int64 v298; // rax@485
  sem_t *v299; // rbx@486
  __int64 v300; // r12@486
  __int64 v301; // r13@486
  signed int v302; // er13@491
  signed __int64 v303; // r12@495
  __int64 *v304; // r8@499
  __int64 v305; // r15@500
  __int64 v306; // r13@502
  __int64 *v307; // rax@504
  __int64 v308; // rsi@508
  __int64 v309; // rdi@508
  int (__fastcall *v310)(__int64); // r15@509
  __int64 v311; // ST48_8@510
  __int64 *v312; // rax@510
  __int64 v313; // r15@514
  __int64 *v314; // rax@514
  _QWORD *v315; // r11@514
  __int64 v316; // rax@518
  __int64 v317; // ST48_8@521
  __int64 v318; // rbx@522
  _QWORD *v319; // r15@522
  __int64 *v320; // rax@523
  const char *v321; // rsi@525
  __int64 v322; // rax@525
  __int64 v323; // r13@525
  __int64 v324; // rax@529
  int v325; // er12@532
  __int64 v326; // rax@532
  signed __int64 v327; // rbx@533
  __int64 v328; // r9@534
  signed __int64 v329; // r11@534
  __int64 v330; // rsi@534
  int v331; // eax@536
  __int64 v332; // r14@537
  sem_t *v333; // rax@537
  sem_t *v334; // r13@537
  __int64 v335; // r11@537
  __int64 v336; // r12@538
  __int64 v337; // rbx@538
  __int64 v338; // rax@538
  __int64 v339; // rsi@540
  __int64 v340; // rdx@540
  __int64 v341; // rax@540
  signed __int64 v342; // r11@540
  __int64 v343; // r15@540
  __int64 *v344; // rax@546
  __int64 v345; // rdx@546
  signed __int64 v346; // ST90_8@549
  __int64 v347; // r15@553
  __int64 v348; // r13@554
  __int64 v349; // r12@554
  signed __int64 v350; // rbx@554
  __int64 *v351; // rax@554
  _QWORD *v352; // r15@564
  __int64 v353; // r12@564
  __int64 *v354; // rax@564
  __int64 v355; // rbx@566
  __int64 v356; // r13@566
  __int64 v357; // rsi@566
  __int64 *v358; // rax@566
  __int64 v359; // rdx@566
  __int64 *v360; // rax@573
  __int64 v361; // r8@576
  __int64 *v362; // rax@577
  __int64 v363; // r8@580
  __int64 v364; // ST48_8@587
  __int64 v365; // r13@589
  __int64 v366; // rbx@589
  int v367; // er15@589
  __int64 *v368; // rax@589
  __int64 v369; // r11@589
  _QWORD *v370; // r11@592
  __int64 v371; // rbx@592
  __int64 v372; // rax@594
  __int64 v373; // r13@595
  sem_t *v374; // r13@602
  int v375; // ebp@603
  signed __int64 v376; // rbx@603
  signed __int64 v377; // r15@603
  int v378; // ebx@606
  sem_t *v379; // rdx@608
  __int64 v380; // rax@609
  int (__fastcall *v381)(__int64 *); // r15@610
  __int64 v382; // ST48_8@611
  __int64 v383; // rax@611
  int (__fastcall *v384)(__int64, __int64); // rax@616
  int v385; // eax@617
  int v386; // er14@617
  unsigned int v387; // edx@619
  int (__fastcall *v388)(__int64 *); // rbp@631
  __int64 v389; // ST48_8@632
  __int64 v390; // ST48_8@634
  __int64 *v391; // rax@634
  __int64 v392; // ST48_8@137
  __int64 *v393; // rax@640
  __int64 v394; // ST48_8@643
  __int64 v395; // r8@643
  __int64 v396; // rdi@644
  int (__fastcall *v397)(__int64, signed __int64, signed __int64, __int64 *); // r10@644
  signed __int64 v398; // rbp@656
  __int64 v399; // rdi@664
  __int64 *v400; // rdx@674
  _QWORD *v401; // rsi@675
  __int64 *v402; // r8@675
  __int64 v403; // rbx@675
  __int64 *v404; // rsi@678
  _QWORD *v405; // r13@678
  __int64 *v406; // rbx@678
  _QWORD *v407; // r14@678
  __int64 *v408; // r10@678
  __int64 *v409; // r9@678
  __int64 v410; // rbp@678
  __int64 *v411; // rdx@679
  _QWORD *v412; // rax@681
  _QWORD *v413; // rcx@683
  __int64 v414; // rdi@686
  __int64 v415; // rax@686
  __int64 v416; // rbp@686
  __int64 *v417; // ST28_8@695
  __int64 *v418; // ST48_8@695
  signed __int64 v419; // r13@697
  __int64 v420; // ST48_8@701
  __int64 *v421; // rax@701
  __int64 v422; // ST48_8@321
  int (__fastcall *v423)(__int64); // rdx@706
  __int64 v424; // ST48_8@707
  __int64 v425; // rax@707
  __int64 v426; // rax@710
  __int64 v427; // ST48_8@712
  int (__fastcall **v428)(_QWORD); // rax@713
  __int64 v429; // ST48_8@715
  __int64 *v430; // rax@718
  __int64 v431; // r8@721
  int v432; // eax@723
  __int64 v433; // rbx@726
  __int64 v434; // ST48_8@727
  __int64 v435; // rdi@731
  __int64 v436; // r13@731
  __int64 v437; // r14@731
  _QWORD *v438; // r8@731
  _QWORD *v439; // rax@733
  __int64 v440; // rsi@744
  __int64 v441; // rsi@744
  __int64 v442; // rsi@744
  __int64 v443; // rdi@744
  __int64 v444; // r13@744
  __int64 v445; // r14@744
  __int64 v446; // ST48_8@139
  __int64 v447; // r12@750
  signed __int64 v448; // rbx@750
  unsigned int v449; // ebx@752
  int v450; // er12@752
  int v451; // esi@753
  signed int v452; // er15@753
  signed int v453; // ebp@755
  int v454; // ebx@758
  int v455; // er9@758
  signed int v456; // er12@758
  int v457; // ebp@762
  unsigned int v458; // ebx@762
  int v459; // er15@766
  signed __int64 v460; // r13@766
  signed int v461; // er15@768
  int v462; // esi@769
  _BYTE *v463; // r13@769
  int v464; // er15@771
  int v465; // esi@772
  int v466; // ebp@772
  int v467; // ebp@774
  int v468; // er13@776
  int v469; // er12@776
  signed __int64 v470; // rbp@776
  signed int v471; // er15@779
  _BYTE *v472; // rbp@786
  __int64 v473; // ST48_8@400
  int (__fastcall *v474)(sem_t *, signed __int64, sem_t *); // r13@792
  int v475; // ecx@793
  __int64 *v476; // rax@794
  _DWORD *v477; // rax@794
  __int64 v478; // rdi@797
  int v479; // er10@797
  __int64 v480; // rdx@797
  int v481; // er14@798
  int v482; // er13@799
  __int64 *v483; // rdi@799
  __int64 v484; // r10@800
  __int64 v485; // rdx@800
  __int64 *v486; // ST48_8@803
  __int64 *v487; // ST28_8@803
  __int64 *v488; // ST48_8@805
  __int64 *v489; // ST28_8@805
  __int64 v490; // ST48_8@402
  __int64 *v491; // rax@816
  __int64 v492; // rdx@817
  signed __int64 v493; // rbx@822
  __int64 v494; // rdi@822
  __int64 v495; // rdi@824
  __int64 *v496; // rbx@826
  __int64 v497; // rdi@826
  __int64 *v498; // rax@833
  int v499; // eax@835
  __int64 *v500; // rax@837
  __int64 *v501; // rax@838
  __int64 v502; // ST48_8@341
  sem_t *v503; // r14@842
  _QWORD *v504; // r12@844
  FILE *v505; // ST98_8@846
  __int64 v506; // ST48_8@849
  __int64 v507; // rax@849
  int (__fastcall **v508)(_QWORD); // rsi@855
  __int64 v509; // ST48_8@857
  sem_t **k; // rbx@865
  __int64 v511; // r14@872
  __int64 v512; // ST48_8@875
  unsigned __int64 v513; // rax@875
  __int64 v514; // rdx@879
  __int64 v515; // rax@882
  __int64 v516; // r13@882
  __int64 v517; // rsi@883
  __int64 *v518; // rax@883
  int (__fastcall **v519)(__int64); // r13@889
  __int64 v520; // ST48_8@891
  __int64 v521; // ST48_8@493
  __int64 v522; // rbp@892
  __int64 v523; // rax@894
  __int64 v524; // r14@894
  __int64 v525; // rsi@895
  int v526; // ebx@895
  __int64 *v527; // ST28_8@902
  __int64 *v528; // ST28_8@905
  __int64 *v529; // ST28_8@908
  __int64 v530; // ST48_8@917
  __int64 v531; // ST48_8@919
  __int64 v532; // ST90_8@479
  __int64 *v533; // ST48_8@479
  int v534; // eax@479
  __int64 v535; // rsi@921
  __int64 **v536; // r12@921
  __int64 v537; // ST48_8@923
  __int64 *v538; // rax@924
  __int64 *v539; // rax@927
  int v540; // er15@928
  __int64 **v541; // r13@928
  __int64 *v542; // rbx@930
  void *v543; // r13@939
  __int64 *v544; // rax@940
  __int64 v545; // r13@945
  sem_t *v546; // rax@945
  sem_t *v547; // r12@945
  __int64 v548; // rax@947
  __int64 v549; // rax@948
  __int64 v550; // ST48_8@953
  __int64 v551; // ST48_8@956
  __int64 v552; // ST48_8@374
  __int64 v553; // ST48_8@303
  __int64 v554; // rcx@959
  __int64 v555; // ST48_8@962
  unsigned __int64 v556; // rax@962
  __int64 v557; // ST48_8@306
  __int64 v558; // rax@967
  __int64 v559; // rbx@967
  __int64 v560; // rdx@968
  int v561; // er15@970
  int v562; // er12@971
  __int64 v563; // r15@976
  sem_t **v564; // rax@977
  _QWORD *v565; // ST48_8@980
  __int64 v566; // r8@981
  __int64 v567; // rsi@983
  __int64 v568; // rax@985
  __int64 *v569; // rax@987
  __int64 v570; // rdx@990
  __int64 v571; // rax@990
  int v572; // eax@991
  int v573; // eax@1000
  __int64 v574; // r15@1005
  signed __int64 v575; // r13@1005
  __int64 v576; // rbx@1005
  __int64 v577; // rax@1006
  __int64 v578; // r12@1006
  const char *v579; // rsi@1007
  __int64 v580; // rax@1011
  const unsigned __int16 **v581; // rax@1013
  __int64 v582; // ST48_8@1022
  __int64 v583; // rdi@1025
  __int64 v584; // r15@1033
  __int64 v585; // rax@1034
  __int64 v586; // rbx@1034
  __int64 v587; // ST48_8@1038
  __int64 v588; // ST48_8@1041
  __int64 v589; // rbx@1043
  signed __int64 v590; // r13@1045
  __int64 v591; // r12@1047
  signed __int64 v592; // rsi@1047
  int v593; // eax@1047
  __int64 v594; // r10@1059
  __int64 v595; // ST48_8@1061
  __int64 *v596; // rax@1061
  __int64 v597; // r13@1062
  __int64 *v598; // r12@1062
  signed __int64 v599; // r11@1062
  __int64 *v600; // rbx@1063
  __int64 v601; // r15@1063
  __int64 v602; // r12@1063
  __int64 v603; // rdi@1068
  const char *v604; // rsi@1072
  __int64 v605; // rax@1072
  void *v606; // r8@1074
  __int64 v607; // rax@1076
  __int64 v608; // rbx@1076
  __int64 v609; // ST58_8@1078
  FILE *v610; // ST98_8@1081
  FILE *v611; // ST98_8@1083
  signed __int64 v612; // r15@1084
  FILE *v613; // ST98_8@1085
  __int64 v614; // ST90_8@1087
  int v615; // eax@1091
  int v616; // eax@1092
  __int64 *v617; // r8@1092
  __int64 v618; // rax@1094
  signed __int64 v619; // r15@1095
  signed __int64 v620; // ST58_8@1100
  void *v621; // rdi@1100
  __int64 v622; // rax@1100
  FILE *v623; // ST98_8@1103
  __int64 *v624; // rax@1103
  FILE *v625; // ST98_8@1105
  __int64 v626; // rdi@1107
  __int64 v627; // rdi@1109
  signed __int64 v628; // rax@1114
  __int64 v629; // r13@1116
  __int64 v630; // r12@1116
  int v631; // eax@1116
  __int64 v632; // rdx@1116
  signed __int64 v633; // ST48_8@1119
  int v634; // er15@1123
  __int64 v635; // r8@1123
  __int64 *v636; // r10@1123
  bool v637; // zf@1123
  bool v638; // sf@1123
  unsigned __int8 v639; // of@1123
  __int64 v640; // rdi@1124
  signed __int64 v641; // rbp@1124
  __int64 *v642; // rdi@1129
  __int64 v643; // r9@1129
  __int64 v644; // r15@1131
  __int64 v645; // rdi@1140
  __int64 v646; // rcx@1144
  __int64 *v647; // r9@1158
  __int64 *v648; // r11@1158
  unsigned __int64 v649; // rsi@1158
  __int64 v650; // r10@1167
  __int64 v651; // rax@1174
  __int64 v652; // rbx@1174
  _QWORD *l; // rax@1175
  int v654; // eax@1177
  __int64 v655; // r12@1181
  __int64 v656; // r14@1182
  signed __int64 v657; // r13@1182
  __int64 v658; // rbp@1182
  __int64 v659; // rdi@1183
  __int64 v660; // rax@1183
  __int64 *v661; // rax@1187
  __int64 v662; // r8@1190
  __int64 v663; // rdi@1192
  __int64 v664; // r14@1192
  __int64 v665; // rdx@1194
  const char *v666; // rsi@1194
  __int64 *v667; // ST48_8@1202
  __int64 v668; // rax@1202
  _QWORD *v669; // rax@1205
  sem_t *v670; // rax@1210
  sem_t *v671; // rax@1213
  __int64 v672; // r10@1217
  signed __int64 v673; // ST48_8@570
  signed __int64 v674; // ST48_8@568
  __int64 v675; // r13@1223
  __int64 v676; // rax@1226
  int (__fastcall *v677)(__int64, signed __int64, signed __int64, __int64, signed __int64, signed __int64); // r15@1228
  __int64 v678; // r13@1230
  __int64 v679; // rdi@1230
  int v680; // eax@1231
  signed int v681; // edx@1232
  __int64 v682; // rdi@1234
  _QWORD *v683; // r10@1252
  int v684; // eax@1253
  _QWORD *v685; // r15@1253
  signed __int64 v686; // rdx@1254
  int v687; // er12@1254
  _QWORD *v688; // r13@1254
  __int64 v689; // r10@1255
  __int64 v690; // ST48_8@1257
  int v691; // eax@1258
  __int64 v692; // r15@1260
  int v693; // eax@1260
  __int64 v694; // rax@1263
  __int64 v695; // r13@1263
  FILE *v696; // rdx@1263
  void *v697; // rdi@1264
  int v698; // eax@1264
  __int64 v699; // rax@1265
  __int64 v700; // r15@1265
  __int64 v701; // rax@1266
  __int64 v702; // rcx@1270
  __int64 v703; // r10@1270
  int (__fastcall *v704)(__int64, const char *); // rax@1271
  const char *v705; // rbx@1272
  __int64 v706; // rdi@1273
  signed __int64 v707; // rdi@1276
  unsigned __int64 v708; // r12@1277
  __int64 v709; // rbp@1286
  __int64 v710; // rax@1289
  __int64 v711; // rbp@1289
  __int64 v712; // rcx@1291
  __int64 v713; // rax@1293
  __int64 v714; // rbx@1293
  unsigned __int64 v715; // rax@1298
  __int64 v716; // r11@1299
  __int64 (__fastcall *v717)(); // rax@1299
  unsigned __int64 v718; // r9@1301
  signed __int64 v719; // r15@1301
  __int64 v720; // r10@1301
  __int64 v721; // rdx@1305
  __int64 v722; // r10@1306
  int (__fastcall *v723)(__int64); // rax@1307
  __int64 **v724; // r8@1310
  __int64 v725; // rdi@1311
  FILE *v726; // r8@1313
  void *v727; // rdi@1314
  int v728; // eax@1314
  FILE *v729; // ST90_8@1317
  signed __int64 v730; // r15@1318
  signed __int64 v731; // rax@1320
  unsigned __int64 v732; // rax@1321
  signed __int64 v733; // rdi@1326
  __int64 v734; // r8@1327
  __int64 v735; // rax@1338
  int (__fastcall *v736)(__int64); // rax@1345
  void (*v737)(void); // r15@1349
  __int64 v738; // r12@1354
  __int64 v739; // r15@1354
  __int64 v740; // r13@1354
  signed __int64 v741; // r11@1354
  signed __int64 v742; // ST90_8@1355
  sem_t *v743; // rax@1355
  sem_t *v744; // r14@1355
  __int64 v745; // rbx@1358
  __int64 v746; // rdi@1358
  sem_t *v747; // rax@1358
  __int64 v748; // ST48_8@1364
  __int64 v749; // ST48_8@1366
  __int64 v750; // ST48_8@1368
  __int64 v751; // rax@1369
  __int64 v752; // rdi@1370
  sem_t *v753; // rax@1372
  __int64 *v754; // rax@1380
  __int64 v755; // rdx@1381
  __int64 v756; // rsi@1381
  signed __int64 v757; // r13@1387
  __int64 v758; // rax@1387
  __int64 v759; // r15@1387
  signed int v760; // er13@1389
  __int64 v761; // rax@1389
  __int64 v762; // rax@1391
  __int64 v763; // rax@1393
  __int64 v764; // rax@1395
  __int64 v765; // rax@1397
  __int64 v766; // rax@1399
  FILE *v767; // ST98_8@1401
  __int64 v768; // rax@1401
  __int64 v769; // rax@1405
  __int64 v770; // ST90_8@1412
  _QWORD *v771; // rbx@1413
  __int64 i; // r12@1413
  __int64 v773; // ST48_8@1417
  __int64 v774; // ST48_8@1420
  char *v775; // rcx@1424
  __int64 v776; // ST90_8@1426
  int v777; // eax@1427
  __int64 *v778; // rsi@1428
  __int64 v779; // rcx@1428
  __int64 v780; // rsi@1429
  __int64 *v781; // rbx@1429
  __int64 v782; // ST90_8@1433
  int v783; // eax@1436
  const char *v784; // rsi@1450
  __int64 v785; // rbp@1452
  __int64 v786; // rdi@1452
  __int64 v787; // rax@1454
  __int64 v788; // rcx@1454
  signed __int64 v789; // ST48_8@1464
  __int64 v790; // r8@1464
  size_t v791; // rdx@1476
  unsigned __int64 v792; // STE0_8@1477
  int v793; // eax@1477
  size_t v794; // rdx@1479
  int v795; // eax@1480
  FILE *v796; // ST58_8@1483
  signed __int64 v797; // rax@1486
  int v798; // eax@1488
  int v799; // eax@1493
  __int64 v800; // rdx@1493
  _QWORD *v801; // r11@1493
  __int64 v802; // rax@1494
  __int64 v803; // rcx@1494
  FILE *v804; // rax@1496
  sem_t *v805; // rax@1496
  __int64 v806; // rdx@1496
  __int64 v807; // rax@1496
  __int64 v808; // r12@1496
  __int64 v809; // rax@1499
  __int64 v810; // ST48_8@1504
  __int64 v811; // ST48_8@1506
  int v812; // eax@1507
  __int64 v813; // ST48_8@28
  int *v814; // rax@1510
  __int64 v815; // r15@1511
  int *v816; // rax@25
  int v817; // er13@1516
  __int64 *v818; // rbx@1516
  __int64 *v819; // rax@1518
  __int64 v820; // ST90_8@692
  __int64 v821; // ST48_8@1522
  __int64 v822; // r8@1524
  __int64 v823; // r13@1524
  __int64 v824; // ST48_8@1527
  __int64 v825; // ST48_8@1532
  __int64 v826; // r8@1532
  __int64 v827; // ST90_8@481
  __int64 *v828; // ST48_8@481
  __int64 v829; // ST48_8@1538
  int v830; // eax@1538
  __int64 v831; // ST48_8@145
  __int64 v832; // ST48_8@178
  __int64 v833; // rax@1545
  __int64 v834; // ST48_8@1548
  __int64 *v835; // rax@1548
  __int64 v836; // rax@1549
  __int64 v837; // rcx@1549
  __int64 v838; // ST48_8@1550
  __int64 v839; // ST48_8@1553
  __int64 v840; // r12@1556
  __int64 v841; // r13@1556
  __int64 *v842; // rax@1556
  __int64 v843; // r15@1560
  __int64 v844; // r15@1562
  int v845; // eax@1562
  __int64 *v846; // rax@1567
  __int64 *v847; // rax@1568
  __int64 v848; // ST48_8@1569
  signed __int64 v849; // ST90_8@1572
  int v850; // eax@1572
  __int64 v851; // rdx@1574
  signed __int64 v852; // ST90_8@1574
  __int64 v853; // ST48_8@527
  int v854; // eax@1578
  __int64 v855; // ST48_8@1586
  signed __int64 v856; // ST90_8@544
  _QWORD *v857; // ST48_8@1588
  unsigned __int64 v858; // ST48_8@1592
  unsigned __int64 v859; // ST48_8@1593
  unsigned __int64 v860; // ST48_8@1595
  signed __int64 v861; // ST90_8@542
  __int64 *v862; // rax@1598
  __int64 v863; // rax@1599
  __int64 *v864; // rax@1605
  __int64 v865; // rdx@1606
  __int64 v866; // rsi@1606
  __int64 v867; // rsi@1609
  __int64 v868; // rbx@1612
  int v869; // eax@1613
  int v870; // eax@1618
  __int64 *v871; // rax@1619
  int v872; // eax@1621
  __int64 v873; // r9@1623
  void *v874; // r10@1627
  char *v875; // r14@1627
  __int64 v876; // r13@1627
  __int64 v877; // rax@1629
  char *v878; // rcx@1629
  signed __int64 v879; // rax@1631
  __int64 v880; // ST48_8@1645
  int v881; // eax@1654
  _QWORD *v882; // ST48_8@599
  _QWORD *v883; // ST48_8@1667
  _QWORD *v884; // ST48_8@1668
  int v885; // eax@1671
  __int64 v886; // rax@1673
  __int64 *v887; // rax@1673
  signed __int64 v888; // rdx@1673
  signed __int64 v889; // ST58_8@1674
  __int64 v890; // rax@1674
  int v891; // eax@1676
  __int64 v892; // rax@1678
  FILE *v893; // rax@1682
  _QWORD *v894; // r10@1686
  int v895; // eax@1692
  __int64 v896; // ST48_8@1693
  __int64 v897; // rax@1693
  __int64 v898; // rcx@1693
  sem_t *v899; // rax@1694
  __int64 v900; // rdx@1698
  __int64 v901; // rax@1700
  __int64 *v902; // rax@1701
  __int64 v903; // ST48_8@1703
  void *v904; // rdi@1707
  char *v905; // r15@1707
  __int64 v906; // r13@1707
  __int64 v907; // rax@1713
  char *v908; // rcx@1713
  __int64 v909; // rax@1719
  __int64 v910; // ST48_8@1724
  __int64 v911; // r8@1724
  __int64 v912; // rcx@1726
  __int64 *v913; // [sp+0h] [bp-188h]@1498
  signed __int64 v914; // [sp+28h] [bp-160h]@8
  __int64 *v915; // [sp+28h] [bp-160h]@500
  __int64 *v916; // [sp+28h] [bp-160h]@602
  __int64 *v917; // [sp+28h] [bp-160h]@678
  signed __int64 v918; // [sp+30h] [bp-158h]@5
  signed __int64 v919; // [sp+38h] [bp-150h]@4
  void *v920; // [sp+40h] [bp-148h]@1
  __int64 v921; // [sp+48h] [bp-140h]@24
  unsigned __int64 v922; // [sp+48h] [bp-140h]@33
  __int64 v923; // [sp+48h] [bp-140h]@60
  __int64 v924; // [sp+48h] [bp-140h]@80
  signed __int64 v925; // [sp+48h] [bp-140h]@123
  __int64 v926; // [sp+48h] [bp-140h]@201
  __int64 v927; // [sp+48h] [bp-140h]@223
  signed __int64 v928; // [sp+48h] [bp-140h]@230
  __int64 v929; // [sp+48h] [bp-140h]@250
  __int64 v930; // [sp+48h] [bp-140h]@263
  __int64 v931; // [sp+48h] [bp-140h]@275
  _QWORD *v932; // [sp+48h] [bp-140h]@311
  signed __int64 v933; // [sp+48h] [bp-140h]@324
  __int64 v934; // [sp+48h] [bp-140h]@353
  __int64 v935; // [sp+48h] [bp-140h]@362
  signed __int64 v936; // [sp+48h] [bp-140h]@388
  __int64 v937; // [sp+48h] [bp-140h]@391
  signed __int64 v938; // [sp+48h] [bp-140h]@393
  signed __int64 v939; // [sp+48h] [bp-140h]@397
  signed __int64 v940; // [sp+48h] [bp-140h]@405
  signed __int64 v941; // [sp+48h] [bp-140h]@409
  signed __int64 v942; // [sp+48h] [bp-140h]@412
  __int64 v943; // [sp+48h] [bp-140h]@419
  __int64 v944; // [sp+48h] [bp-140h]@422
  __int64 v945; // [sp+48h] [bp-140h]@426
  signed __int64 v946; // [sp+48h] [bp-140h]@430
  signed __int64 v947; // [sp+48h] [bp-140h]@432
  sem_t *v948; // [sp+48h] [bp-140h]@436
  __int64 *v949; // [sp+48h] [bp-140h]@475
  sem_t *v950; // [sp+48h] [bp-140h]@500
  __int64 v951; // [sp+48h] [bp-140h]@503
  __int64 v952; // [sp+48h] [bp-140h]@512
  signed __int64 v953; // [sp+48h] [bp-140h]@514
  __int64 v954; // [sp+48h] [bp-140h]@523
  __int64 v955; // [sp+48h] [bp-140h]@525
  __int64 v956; // [sp+48h] [bp-140h]@529
  __int64 v957; // [sp+48h] [bp-140h]@535
  __int64 v958; // [sp+48h] [bp-140h]@537
  sem_t *v959; // [sp+48h] [bp-140h]@554
  _QWORD *v960; // [sp+48h] [bp-140h]@564
  __int64 v961; // [sp+48h] [bp-140h]@566
  signed __int64 v962; // [sp+48h] [bp-140h]@573
  signed __int64 v963; // [sp+48h] [bp-140h]@577
  __int64 v964; // [sp+48h] [bp-140h]@589
  __int64 v965; // [sp+48h] [bp-140h]@617
  __int64 v966; // [sp+48h] [bp-140h]@640
  __int64 *v967; // [sp+48h] [bp-140h]@678
  __int64 v968; // [sp+48h] [bp-140h]@723
  __int64 v969; // [sp+48h] [bp-140h]@837
  __int64 v970; // [sp+48h] [bp-140h]@882
  __int64 **v971; // [sp+48h] [bp-140h]@927
  signed __int64 v972; // [sp+48h] [bp-140h]@945
  signed __int64 v973; // [sp+48h] [bp-140h]@957
  _QWORD *v974; // [sp+48h] [bp-140h]@967
  __int64 v975; // [sp+48h] [bp-140h]@984
  __int64 v976; // [sp+48h] [bp-140h]@986
  sem_t *v977; // [sp+48h] [bp-140h]@1000
  __int64 v978; // [sp+48h] [bp-140h]@1012
  __int64 v979; // [sp+48h] [bp-140h]@1034
  __int64 v980; // [sp+48h] [bp-140h]@1035
  __int64 *v981; // [sp+48h] [bp-140h]@1062
  signed __int64 v982; // [sp+48h] [bp-140h]@1116
  _QWORD *v983; // [sp+48h] [bp-140h]@1174
  signed __int64 v984; // [sp+48h] [bp-140h]@1177
  __int64 v985; // [sp+48h] [bp-140h]@1182
  signed __int64 v986; // [sp+48h] [bp-140h]@1187
  __int64 v987; // [sp+48h] [bp-140h]@1194
  __int64 v988; // [sp+48h] [bp-140h]@1223
  sem_t *v989; // [sp+48h] [bp-140h]@1226
  signed __int64 v990; // [sp+48h] [bp-140h]@1260
  __int64 v991; // [sp+48h] [bp-140h]@1262
  __int64 v992; // [sp+48h] [bp-140h]@1354
  __int64 v993; // [sp+48h] [bp-140h]@1376
  FILE *v994; // [sp+48h] [bp-140h]@1387
  _QWORD *j; // [sp+48h] [bp-140h]@1389
  __int64 v996; // [sp+48h] [bp-140h]@1509
  __int64 v997; // [sp+48h] [bp-140h]@1518
  __int64 v998; // [sp+48h] [bp-140h]@1524
  sem_t *v999; // [sp+48h] [bp-140h]@1556
  __int64 v1000; // [sp+48h] [bp-140h]@1560
  signed __int64 v1001; // [sp+48h] [bp-140h]@1562
  __int64 v1002; // [sp+48h] [bp-140h]@1567
  __int64 v1003; // [sp+48h] [bp-140h]@1577
  __int64 v1004; // [sp+50h] [bp-138h]@4
  int v1005; // [sp+58h] [bp-130h]@7
  signed int v1006; // [sp+58h] [bp-130h]@1067
  FILE *v1007; // [sp+58h] [bp-130h]@1272
  signed __int64 v1008; // [sp+58h] [bp-130h]@1671
  __int64 v1009; // [sp+60h] [bp-128h]@4
  __int64 v1010; // [sp+68h] [bp-120h]@7
  __int64 PyObject_co; // [sp+70h] [bp-118h]@4
  unsigned __int64 v1012; // [sp+7Ch] [bp-10Ch]@7
  signed int v1013; // [sp+84h] [bp-104h]@7
  signed __int64 v1014; // [sp+88h] [bp-100h]@4
  unsigned int v1015; // [sp+90h] [bp-F8h]@436
  __int64 v1016; // [sp+90h] [bp-F8h]@475
  __int64 v1017; // [sp+90h] [bp-F8h]@538
  signed __int64 v1018; // [sp+90h] [bp-F8h]@546
  signed __int64 v1019; // [sp+90h] [bp-F8h]@794
  signed __int64 v1020; // [sp+90h] [bp-F8h]@1012
  __int64 v1021; // [sp+90h] [bp-F8h]@1062
  signed __int64 v1022; // [sp+90h] [bp-F8h]@1262
  signed __int64 v1023; // [sp+90h] [bp-F8h]@1272
  FILE *v1024; // [sp+90h] [bp-F8h]@1314
  signed __int64 v1025; // [sp+90h] [bp-F8h]@1358
  signed __int64 v1026; // [sp+90h] [bp-F8h]@1369
  signed __int64 v1027; // [sp+90h] [bp-F8h]@1387
  sem_t *v1028; // [sp+90h] [bp-F8h]@1413
  __int64 v1029; // [sp+90h] [bp-F8h]@1427
  __int64 v1030; // [sp+90h] [bp-F8h]@1436
  const char *v1031; // [sp+90h] [bp-F8h]@1618
  __int64 v1032; // [sp+90h] [bp-F8h]@1699
  FILE *stream; // [sp+98h] [bp-F0h]@1070
  FILE *streama; // [sp+98h] [bp-F0h]@1075
  FILE *streamb; // [sp+98h] [bp-F0h]@1089
  FILE *streamc; // [sp+98h] [bp-F0h]@1090
  signed int streamd; // [sp+98h] [bp-F0h]@1269
  FILE *streame; // [sp+98h] [bp-F0h]@1318
  FILE *streamf; // [sp+98h] [bp-F0h]@1360
  FILE *streamg; // [sp+98h] [bp-F0h]@1389
  FILE *streamh; // [sp+98h] [bp-F0h]@1405
  FILE *streami; // [sp+98h] [bp-F0h]@1496
  FILE *streamj; // [sp+98h] [bp-F0h]@1676
  __int64 v1044; // [sp+A0h] [bp-E8h]@1089
  __int64 v1045; // [sp+A0h] [bp-E8h]@1091
  int v1046; // [sp+A0h] [bp-E8h]@1272
  __int64 v1047; // [sp+A0h] [bp-E8h]@1496
  __int64 v1048; // [sp+A8h] [bp-E0h]@1300
  __int64 v1049; // [sp+B0h] [bp-D8h]@1300
  unsigned __int64 v1050; // [sp+B8h] [bp-D0h]@1301
  __int64 v1051; // [sp+C0h] [bp-C8h]@1323
  signed __int64 v1052; // [sp+C8h] [bp-C0h]@1299
  __int64 *v1053; // [sp+D8h] [bp-B0h]@1272
  __int64 v1054; // [sp+F0h] [bp-98h]@1124
  __int64 *v1055; // [sp+100h] [bp-88h]@1124
  unsigned __int64 v1056; // [sp+110h] [bp-78h]@1124
  __int64 v1057; // [sp+120h] [bp-68h]@675
  __int64 *v1058; // [sp+130h] [bp-58h]@675
  __int64 v1059; // [sp+140h] [bp-48h]@33

  PyFrameObject = a1;
  v920 = PyThreadState_Current;
  if ( !a1 )
    return 0LL;
  v3 = (signed int)a2;
  v4 = (unsigned int)(*((_DWORD *)PyThreadState_Current + 6) + 1);
  v639 = __OFSUB__((_DWORD)v4, Py_CheckRecursionLimit);
  v637 = (_DWORD)v4 == Py_CheckRecursionLimit;
  v638 = (signed int)v4 - Py_CheckRecursionLimit < 0;
  *((_DWORD *)PyThreadState_Current + 6) = v4;
  if ( !((unsigned __int8)(v638 ^ v639) | v637) )
  {
    if ( Py_CheckRecursiveCall(&s1) )
      return 0LL;
  }
  v5 = *((_DWORD *)v920 + 8);
  *((_QWORD *)v920 + 2) = a1;
  if ( v5 )
  {
    if ( (v626 = *((_QWORD *)v920 + 6)) != 0
      && (a2 = (const char *)*((_QWORD *)v920 + 8), sub_563E80(v626, a2, PyFrameObject, 0LL, &Py_NoneStruct))
      || (v627 = *((_QWORD *)v920 + 5)) != 0
      && (a2 = (const char *)*((_QWORD *)v920 + 7), sub_563E80(v627, a2, PyFrameObject, 0LL, &Py_NoneStruct)) )
    {
      v66 = 0LL;
      goto LABEL_75;
    }
  }
  Code = *(_QWORD *)(PyFrameObject + 32);
  v919 = PyFrameObject + 376;
  PyObject_co = *(_QWORD *)(PyFrameObject + 32);
  v1004 = *(_QWORD *)(Code + 48);
  v1009 = *(_QWORD *)(Code + 40);
  v1014 = PyFrameObject + 8LL * *(_DWORD *)(Code + 20) + 376;
  if ( *(_BYTE *)(Code + 30) & 0x10 )
  {
    v7 = *(sem_t **)(PyFrameObject + 32);
    v918 = PyShield_Decrypt(PyObject_co);
    v9 = v918;
  }
  else
  {
    v7 = *(sem_t **)(PyFrameObject + 32);
    v8 = *(_QWORD *)(PyObject_co + 72);
    v918 = v8 + 36;
    v9 = v8 + 36;
  }
  v10 = *(_DWORD *)(PyFrameObject + 120);
  v11 = *(_QWORD *)(PyFrameObject + 72);
  *(_QWORD *)(PyFrameObject + 72) = 0LL;
  next_instr = v9 + v10 + 1;
  if ( !v3 )
  {
    v1010 = 0LL;
    v1013 = -1;
    v1012 = 0xFFFFFFFFLL;
    v13 = &Py_NoneStruct;
    v1005 = 0;
    goto LABEL_8;
  }
  if ( !*((_QWORD *)PyThreadState_Current + 9) )
  {
    v818 = &Py_NoneStruct;
    v817 = 0;
    v1010 = 0LL;
    v1012 = 0xFFFFFFFFLL;
    v1013 = -1;
    goto LABEL_1517;
  }
  v7 = (sem_t *)v11;
  v304 = &Py_NoneStruct;
  v1005 = 0;
  v1010 = 0LL;
  v1012 = 0xFFFFFFFFLL;
  v1013 = -1;
LABEL_500:
  v950 = v7;
  v915 = v304;
  PyTraceBack_Here(PyFrameObject, a2);
  v62 = v915;
  v305 = *((_QWORD *)v920 + 6);
  if ( !v305 )
    goto LABEL_501;
  v522 = *((_QWORD *)v920 + 8);
  PyErr_Fetch(&v1057, &v1058, &v1059);
  if ( !v1058 )
  {
    ++Py_NoneStruct;
    v1058 = &Py_NoneStruct;
  }
  v523 = PyTuple_Pack(3);
  v524 = v523;
  if ( v523 )
  {
    v525 = v522;
    v526 = sub_5B63D0(v305, v522, PyFrameObject, 1LL, v523);
    v62 = v915;
    v9 = *(_QWORD *)v524 - 1LL;
    v637 = *(_QWORD *)v524 == 1LL;
    *(_QWORD *)v524 = v9;
    if ( v637 )
    {
      (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v524 + 8) + 48LL))(v524, v522);
      v62 = v915;
    }
    if ( v526 )
    {
      if ( v1057 )
      {
        v637 = (*(_QWORD *)v1057)-- == 1LL;
        if ( v637 )
        {
          v527 = v62;
          (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v1057 + 8) + 48LL))(v1057, v522);
          v62 = v527;
        }
      }
      if ( v1058 )
      {
        v525 = *v1058;
        v637 = (*v1058)-- == 1;
        if ( v637 )
        {
          v528 = v62;
          (*(void (**)(void))(v1058[1] + 48))();
          v62 = v528;
        }
      }
      v4 = v1059;
      if ( v1059 )
      {
        v637 = (*(_QWORD *)v1059)-- == 1LL;
        if ( v637 )
        {
          v529 = v62;
          (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v1059 + 8) + 48LL))(v1059, v525);
          v62 = v529;
        }
      }
      goto LABEL_501;
    }
    v950 = v7;
    v915 = v62;
  }
  PyErr_Restore(v1057, v1058, v1059);
  v62 = v915;
  v7 = v950;
LABEL_501:
  v63 = 2;
LABEL_68:
  v64 = *(_DWORD *)(PyFrameObject + 128);
  if ( v64 > 0 )
  {
    v916 = v62;
    v374 = v7;
    while ( 1 )
    {
      v375 = v64 - 1;
      v7 = (sem_t *)v375;
      v10 = 3LL * v375;
      v376 = PyFrameObject + 12LL * v375 + 128;
      v377 = PyFrameObject + 12LL * v375 + 132;
      if ( *(_DWORD *)v377 == 120 && v63 == 32 )
      {
        v7 = (sem_t *)v1010;
        v13 = v916;
        v11 = (signed __int64)v374;
        next_instr = *(_QWORD *)(v1010 + 16) + v918;
        v4 = *(_QWORD *)v1010 - 1LL;
        *(_QWORD *)v1010 = v4;
        if ( !v4 )
        {
          (*(void (**)(void))(*(_QWORD *)(v1010 + 8) + 48LL))();
          v13 = v916;
        }
        goto LABEL_8;
      }
      v65 = *(_QWORD *)(PyFrameObject + 64);
      *(_DWORD *)(PyFrameObject + 128) = v375;
      v9 = *(_DWORD *)(PyFrameObject + 12LL * v375 + 140);
      if ( (signed int)v9 < (signed int)(((signed __int64)v374 - v65) >> 3) )
      {
        v398 = (signed __int64)(&v374[-1].__align + 3);
        v7 = (sem_t *)*(&v374[-1].__align + 3);
        v374 = (sem_t *)((char *)v374 - 8);
        if ( v7 )
        {
          v637 = v7->__align-- == 1;
          if ( v637 )
          {
            (*(void (**)(void))(*(&v7->__align + 1) + 48))();
            v65 = *(_QWORD *)(PyFrameObject + 64);
            v9 = *(_DWORD *)(v377 + 8);
          }
          else
          {
            v65 = *(_QWORD *)(PyFrameObject + 64);
            v9 = *(_DWORD *)(v377 + 8);
          }
        }
        if ( (signed int)v9 < (signed int)((v398 - v65) >> 3) )
        {
          v7 = *(sem_t **)(v398 - 8);
          v374 = (sem_t *)(v398 - 8);
          if ( v7 )
          {
            v637 = v7->__align-- == 1;
            if ( v637 )
            {
              (*(void (**)(void))(*(&v7->__align + 1) + 48))();
              v65 = *(_QWORD *)(PyFrameObject + 64);
              v9 = *(_DWORD *)(v377 + 8);
            }
            else
            {
              v65 = *(_QWORD *)(PyFrameObject + 64);
              v9 = *(_DWORD *)(v377 + 8);
            }
          }
          v4 = ((signed __int64)v374 - v65) >> 3;
          if ( (signed int)v9 < (signed int)v4 )
          {
            v399 = *(_QWORD *)(v398 - 16);
            v374 = (sem_t *)(v398 - 16);
            if ( !v399 )
              goto LABEL_668;
LABEL_665:
            v637 = (*(_QWORD *)v399)-- == 1LL;
            if ( v637 )
              (*(void (**)(void))(*(_QWORD *)(v399 + 8) + 48LL))();
            v65 = *(_QWORD *)(PyFrameObject + 64);
            v9 = *(_DWORD *)(v377 + 8);
LABEL_668:
            while ( 1 )
            {
              v7 = (sem_t *)(((signed __int64)v374 - v65) >> 3);
              if ( (signed int)v9 >= (signed int)v7 )
                break;
              v374 = (sem_t *)((char *)v374 - 8);
              v399 = v374->__align;
              if ( v374->__align )
                goto LABEL_665;
            }
          }
        }
      }
      v378 = *(_DWORD *)(v376 + 4);
      if ( v378 == 120 )
      {
        if ( v63 == 16 )
        {
          v379 = v374;
          v13 = v916;
          next_instr = v918 + *(_DWORD *)(v377 + 4);
          v11 = (signed __int64)v379;
          goto LABEL_8;
        }
        goto LABEL_635;
      }
      if ( v378 == 122 )
        goto LABEL_674;
      if ( v378 != 121 )
      {
        if ( v378 == 143 )
        {
LABEL_674:
          v13 = v916;
          v400 = (__int64 *)v374;
          if ( v63 == 2 )
            goto LABEL_675;
          if ( v63 & 0x28 )
          {
            v400 = &v374->__align + 1;
            v374->__align = v1010;
          }
          v7 = (sem_t *)(unsigned int)v63;
          v667 = v400;
          LODWORD(v668) = PyInt_FromLong((unsigned int)v63);
          v13 = v916;
          v11 = (signed __int64)(v667 + 1);
          *v667 = v668;
          goto LABEL_699;
        }
        goto LABEL_635;
      }
      if ( v63 == 2 )
        break;
LABEL_635:
      v64 = *(_DWORD *)(PyFrameObject + 128);
      if ( v64 <= 0 )
      {
        v7 = v374;
        goto LABEL_70;
      }
    }
    v13 = v916;
    v400 = (__int64 *)v374;
LABEL_675:
    v401 = PyThreadState_Current;
    v1057 = *((_QWORD *)PyThreadState_Current + 9);
    v1058 = (__int64 *)*((_QWORD *)PyThreadState_Current + 10);
    v402 = v1058;
    v403 = *((_QWORD *)PyThreadState_Current + 11);
    v1059 = *((_QWORD *)PyThreadState_Current + 11);
    *((_QWORD *)PyThreadState_Current + 9) = 0LL;
    v401[10] = 0LL;
    v401[11] = 0LL;
    if ( !v402 )
    {
      v1058 = &Py_NoneStruct;
      ++Py_NoneStruct;
    }
    LOBYTE(v7) = *(_DWORD *)v377 == 121;
    LOBYTE(v4) = (unsigned __int8)v7 | (*(_DWORD *)v377 == 143);
    if ( (_BYTE)v4 )
    {
      v967 = v400;
      v404 = (__int64 *)&v1058;
      v917 = v13;
      PyErr_NormalizeException(&v1057, &v1058, &v1059);
      v405 = (_QWORD *)v1059;
      v406 = v1058;
      v407 = (_QWORD *)v1057;
      v408 = v917;
      v409 = v967;
      v410 = *((_QWORD *)v920 + 2);
      if ( !*(_QWORD *)(v410 + 88) )
      {
        v411 = (__int64 *)*((_QWORD *)v920 + 12);
        if ( !v411 )
        {
          ++Py_NoneStruct;
          *((_QWORD *)v920 + 12) = &Py_NoneStruct;
          v411 = &Py_NoneStruct;
        }
        ++*v411;
        v412 = (_QWORD *)*((_QWORD *)v920 + 13);
        if ( v412 )
          ++*v412;
        v413 = (_QWORD *)*((_QWORD *)v920 + 14);
        if ( v413 )
          ++*v413;
        *(_QWORD *)(v410 + 88) = *((_QWORD *)v920 + 12);
        *(_QWORD *)(v410 + 96) = *((_QWORD *)v920 + 13);
        v404 = (__int64 *)*((_QWORD *)v920 + 14);
        *(_QWORD *)(v410 + 104) = v404;
      }
      v414 = *((_QWORD *)v920 + 12);
      v415 = *((_QWORD *)v920 + 13);
      v416 = *((_QWORD *)v920 + 14);
      ++*v407;
      if ( v406 )
        ++*v406;
      if ( v405 )
        ++*v405;
      *((_QWORD *)v920 + 12) = v407;
      *((_QWORD *)v920 + 13) = v406;
      *((_QWORD *)v920 + 14) = v405;
      if ( v414 )
      {
        v637 = (*(_QWORD *)v414)-- == 1LL;
        if ( v637 )
        {
          v820 = v415;
          (*(void (__fastcall **)(__int64, __int64 *))(*(_QWORD *)(v414 + 8) + 48LL))(v414, v404);
          v415 = v820;
          v409 = v967;
          v408 = v917;
        }
      }
      if ( v415 )
      {
        v637 = (*(_QWORD *)v415)-- == 1LL;
        if ( v637 )
        {
          v488 = v409;
          v489 = v408;
          (*(void (__fastcall **)(__int64, __int64 *))(*(_QWORD *)(v415 + 8) + 48LL))(v415, v404);
          v409 = v488;
          v408 = v489;
        }
      }
      if ( v416 )
      {
        v637 = (*(_QWORD *)v416)-- == 1LL;
        if ( v637 )
        {
          v486 = v409;
          v487 = v408;
          (*(void (__fastcall **)(__int64))(*(_QWORD *)(v416 + 8) + 48LL))(v416);
          v409 = v486;
          v408 = v487;
        }
      }
      v417 = v408;
      v418 = v409;
      PySys_SetObject("exc_type");
      PySys_SetObject(&unk_61725B);
      v7 = (sem_t *)"exc_traceback";
      PySys_SetObject("exc_traceback");
      v403 = v1059;
      v13 = v417;
      v400 = v418;
    }
    if ( v403 )
    {
      v419 = (signed __int64)(v400 + 1);
      *v400 = v403;
    }
    else
    {
      ++Py_NoneStruct;
      v419 = (signed __int64)(v400 + 1);
      *v400 = (__int64)&Py_NoneStruct;
    }
    v11 = v419 + 16;
    *(_QWORD *)v419 = v1058;
    v9 = v1057;
    *(_QWORD *)(v419 + 8) = v1057;
LABEL_699:
    next_instr = v918 + *(_DWORD *)(v377 + 4);
LABEL_8:
    v14 = (sem_t *)v11;
    v15 = v13;
    v914 = PyFrameObject;
LABEL_9:
    v16 = (__int64)v14;
LABEL_10:
    if ( --Py_Ticker < 0 )
      goto LABEL_20;
    while ( 2 )
    {
      a2 = (const char *)(unsigned int)dword_93C2B0;
      v17 = next_instr - v918;
      v637 = dword_93C2B0 == 0;
      v18 = next_instr - v918;
      *(_DWORD *)(v914 + 120) = next_instr - v918;
      if ( v637 )
      {
        opcode = *(_BYTE *)next_instr;
        if ( opcode > 89 )
        {
          v20 = next_instr + 3;
          v21 = (*(_BYTE *)(next_instr + 2) << 8) + *(_BYTE *)(next_instr + 1);
          goto LABEL_14;
        }
LABEL_84:
        v20 = next_instr + 1;
        v21 = 0;
        goto LABEL_14;
      }
      if ( *((_QWORD *)v920 + 6) && !*((_DWORD *)v920 + 7) )
      {
        v4 = (signed __int64)v920;
        *(_QWORD *)(v914 + 72) = v16;
        v9 = *(_DWORD *)(v914 + 124);
        v396 = *((_QWORD *)v920 + 8);
        v397 = (int (__fastcall *)(__int64, signed __int64, signed __int64, __int64 *))*((_QWORD *)v920 + 6);
        if ( v17 >= SHIDWORD(v1012) && v17 < (signed int)v1012 )
          goto LABEL_646;
        v447 = *(_QWORD *)(v914 + 32);
        v10 = *(_QWORD *)(v447 + 104);
        v9 = *(_DWORD *)(v447 + 96);
        v4 = v10 + 36;
        v448 = *(_QWORD *)(v10 + 16) / 2LL;
        a2 = (const char *)(unsigned int)v448;
        if ( (signed int)v448 <= 0 )
        {
          HIDWORD(v1012) = 0;
        }
        else
        {
          v1012 = *(_BYTE *)(v10 + 36);
          if ( v17 < *(_BYTE *)(v10 + 36) )
          {
            LODWORD(v1012) = 0;
            goto LABEL_781;
          }
          v449 = v448 - 1;
          v450 = v449 & 3;
          if ( !(v449 & 3) )
          {
            v10 = *(_BYTE *)(v10 + 36);
            v458 = 0;
            goto LABEL_776;
          }
          v451 = *(_BYTE *)(v10 + 37);
          v4 = v10 + 38;
          v452 = 0;
          if ( (_BYTE)v451 )
            v452 = *(_BYTE *)(v10 + 36);
          v453 = v1012 + *(_BYTE *)(v10 + 38);
          v9 = (unsigned int)(v451 + v9);
          HIDWORD(v1012) = v452;
          a2 = (const char *)v449;
          if ( v18 < v453 )
            goto LABEL_781;
          LODWORD(v1012) = v1012 + *(_BYTE *)(v10 + 38);
          if ( v450 == 1 )
          {
            v10 = (unsigned int)v453;
            v458 = v452;
            goto LABEL_776;
          }
          if ( v450 != 2 )
          {
            v454 = *(_BYTE *)(v10 + 39);
            v4 = v10 + 40;
            v455 = *(_BYTE *)(v10 + 40);
            v456 = v452;
            if ( (_BYTE)v454 )
              v456 = v453;
            v10 = (unsigned int)(v453 + v455);
            v9 = (unsigned int)(v454 + v9);
            a2 = (const char *)(unsigned int)((_DWORD)a2 - 1);
            HIDWORD(v1012) = v456;
            if ( v18 >= (signed int)v10 )
            {
              LODWORD(v1012) = v10;
              goto LABEL_762;
            }
LABEL_781:
            if ( (signed int)a2 - 1 >= 0 )
            {
              LODWORD(v1012) = *(_BYTE *)v4 + (_DWORD)v1012;
              if ( !*(_BYTE *)(v4 + 1) && (signed int)a2 - 2 >= 0 )
              {
                LODWORD(v1012) = *(_BYTE *)(v4 + 2) + (_DWORD)v1012;
                if ( !*(_BYTE *)(v4 + 3) )
                {
                  a2 = (const char *)(unsigned int)((_DWORD)a2 - 3);
                  if ( (signed int)a2 >= 0 )
                  {
                    v472 = (_BYTE *)(v4 + 6);
                    LODWORD(v1012) = *(_BYTE *)(v4 + 4) + (_DWORD)v1012;
                    if ( !*(_BYTE *)(v4 + 5) )
                    {
                      do
                      {
                        a2 = (const char *)(unsigned int)((_DWORD)a2 - 1);
                        if ( (signed int)a2 < 0 )
                          break;
                        v4 = *v472;
                        v472 += 2;
                        LODWORD(v1012) = v4 + v1012;
                      }
                      while ( !*(v472 - 1) );
                    }
                  }
                }
              }
            }
LABEL_646:
            if ( v17 == HIDWORD(v1012) || v17 < v1013 )
            {
              a2 = (const char *)v914;
              v433 = *(_QWORD *)(v914 + 112);
              *(_DWORD *)(v914 + 124) = v9;
              v9 = *(_DWORD *)(v433 + 28);
              if ( (_DWORD)v9 )
              {
                v1005 = 0;
              }
              else
              {
                v434 = v16;
                *(_DWORD *)(v433 + 28) = 1;
                *(_DWORD *)(v433 + 32) = 0;
                v1005 = v397(v396, v914, 2LL, &Py_NoneStruct);
                v16 = v434;
                if ( *(_QWORD *)(v433 + 48) )
                  v10 = 1LL;
                else
                  v10 = *(_QWORD *)(v433 + 40) != 0LL;
                --*(_DWORD *)(v433 + 28);
                *(_DWORD *)(v433 + 32) = v10;
              }
              v7 = *(sem_t **)(v914 + 72);
              v1013 = *(_DWORD *)(v914 + 120);
            }
            else
            {
              v1013 = v17;
              v7 = (sem_t *)v16;
              v1005 = 0;
            }
            next_instr = v918 + v1013;
            if ( v7 )
            {
              *(_QWORD *)(v914 + 72) = 0LL;
              v16 = (__int64)v7;
            }
            if ( v1005 )
              goto LABEL_652;
            goto LABEL_347;
          }
LABEL_762:
          v457 = *(_BYTE *)(v4 + 1);
          v458 = HIDWORD(v1012);
          if ( (_BYTE)v457 )
            v458 = v1012;
          v4 += 2LL;
          v9 = (unsigned int)(v457 + v9);
          a2 = (const char *)(unsigned int)((_DWORD)a2 - 1);
          HIDWORD(v1012) = v458;
          v10 = (unsigned int)v1012 + *(_BYTE *)v4;
          if ( v18 < (signed int)v10 )
            goto LABEL_781;
          while ( 1 )
          {
LABEL_776:
            v468 = *(_BYTE *)(v4 + 1);
            v469 = (_DWORD)a2 - 1;
            v470 = v4 + 2;
            a2 = (const char *)(unsigned int)((_DWORD)a2 - 1);
            v4 += 2LL;
            if ( (_BYTE)v468 )
              v458 = v10;
            v9 = (unsigned int)(v468 + v9);
            if ( !v469 )
              break;
            v471 = v10 + *(_BYTE *)v470;
            if ( v18 < v471 )
              goto LABEL_1730;
            v10 = (unsigned int)v471;
            v459 = *(_BYTE *)(v470 + 1);
            v460 = v4 + 2;
            v4 += 2LL;
            if ( (_BYTE)v459 )
              v458 = v10;
            v9 = (unsigned int)(v459 + v9);
            a2 = (const char *)(unsigned int)((_DWORD)a2 - 1);
            v461 = v10 + *(_BYTE *)v460;
            if ( v18 < v461 )
              goto LABEL_1730;
            v462 = *(_BYTE *)(v460 + 1);
            v463 = (_BYTE *)(v460 + 2);
            v10 = (unsigned int)v461;
            v4 = v470 + 4;
            if ( (_BYTE)v462 )
              v458 = v461;
            v9 = (unsigned int)(v462 + v9);
            a2 = (const char *)(unsigned int)(v469 - 2);
            v464 = v461 + *v463;
            if ( v18 < v464 )
              goto LABEL_1730;
            v465 = *(_BYTE *)(v470 + 5);
            v4 = v470 + 6;
            v466 = *(_BYTE *)(v470 + 6);
            v10 = (unsigned int)v464;
            if ( (_BYTE)v465 )
              v458 = v464;
            v467 = v464 + v466;
            v9 = (unsigned int)(v465 + v9);
            a2 = (const char *)(unsigned int)(v469 - 3);
            if ( v18 < v467 )
            {
LABEL_1730:
              v1012 = __PAIR__(v458, (unsigned int)v10);
              goto LABEL_781;
            }
            v10 = (unsigned int)v467;
          }
          HIDWORD(v1012) = v458;
        }
        LODWORD(v1012) = 0x7FFFFFFF;
        goto LABEL_646;
      }
LABEL_347:
      opcode = *(_BYTE *)next_instr;
      if ( opcode <= 89 )
        goto LABEL_84;
      v20 = next_instr + 3;
      v21 = (*(_BYTE *)(next_instr + 2) << 8) + *(_BYTE *)(next_instr + 1);
LABEL_14:
      switch ( opcode )
      {
        case 124:
        case 200:
          v7 = (sem_t *)v21;
          v15 = *(__int64 **)(v919 + 8LL * v21);
          if ( v15 )
            goto LABEL_16;
          v946 = v16;
          v535 = v21;
          goto LABEL_1545;
        case 106:
          v22 = *(_QWORD *)(v16 - 8);
          v23 = v16;
          v7 = *(sem_t **)(v16 - 8);
          LODWORD(v24) = PyObject_GetAttr(v7, *(_QWORD *)(v1004 + 8LL * v21 + 24));
          v637 = (*(_QWORD *)v22)-- == 1LL;
          v15 = v24;
          v16 = v23;
          if ( !v637 )
            goto LABEL_18;
          goto LABEL_512;
        case 131:
          v28 = BYTE1(v21);
          v29 = (unsigned __int8)v21;
          v1059 = v16;
          v30 = 2 * BYTE1(v21);
          v31 = v28;
          v32 = v30 + v29;
          v33 = -8 - 8LL * (v30 + v29);
          v34 = *(_QWORD *)(v16 + v33);
          v922 = v16 + v33;
          v35 = *(void **)(v34 + 8);
          if ( v35 == &PyCFunction_Type )
          {
            if ( !v28 )
            {
              v36 = *(_QWORD *)(v34 + 16);
              v37 = PyThreadState_Current;
              v38 = *(_DWORD *)(v36 + 16);
              if ( !(v38 & 0xC) )
              {
                LODWORD(v39) = PyTuple_New(v29);
                if ( v39 )
                {
                  v40 = v39;
                  if ( v29 )
                  {
                    v41 = v1059;
                    v42 = v1059 - 16;
                    v1059 -= 8LL;
                    *(_QWORD *)(v39 + 8LL * (v29 - 1) + 24) = *(_QWORD *)(v41 - 8);
                    if ( v29 != 1 )
                    {
                      v1059 = v42;
                      *(_QWORD *)(v39 + 8LL * (v29 - 2) + 24) = *(_QWORD *)(v41 - 16);
                      if ( v29 != 2 )
                      {
                        v1059 = v41 - 24;
                        v43 = v29 - 4;
                        *(_QWORD *)(v39 + 8LL * (v29 - 3) + 24) = *(_QWORD *)(v41 - 24);
                        if ( v29 != 3 )
                        {
                          v1059 = v41 - 32;
                          v44 = *(_QWORD *)(v41 - 32);
                          v45 = v29 - 5;
                          v46 = (__int64 *)(v41 - 40);
                          for ( *(_QWORD *)(v39 + 8LL * v43 + 24) = v44; v45 != -1; *(_QWORD *)(v39 + 8 * v48 + 24) = v47 )
                          {
                            v1059 = (__int64)v46;
                            v47 = *v46;
                            v48 = v45--;
                            --v46;
                          }
                        }
                      }
                    }
                    v40 = v39;
                  }
                }
                else
                {
                  v40 = 0LL;
                }
                if ( *((_DWORD *)v37 + 8) && (v7 = (sem_t *)*((_QWORD *)v37 + 5)) != 0LL )
                {
                  v49 = *((_QWORD *)v37 + 7);
                  if ( sub_5B63D0(v7, v49, *((_QWORD *)v37 + 2), 4LL, v34) )
                  {
LABEL_821:
                    v15 = 0LL;
                    goto LABEL_46;
                  }
                  v49 = v40;
                  LODWORD(v491) = PyCFunction_Call(v34, v40, 0LL);
                  v7 = (sem_t *)*((_QWORD *)v37 + 5);
                  v15 = v491;
                  if ( v7 )
                  {
                    v492 = *((_QWORD *)v37 + 2);
                    v49 = *((_QWORD *)v37 + 7);
                    if ( !v491 )
                    {
                      sub_563E80(v7, v49, v492, 5LL, v34);
                      goto LABEL_46;
                    }
                    if ( sub_5B63D0(v7, v49, v492, 6LL, v34) )
                    {
                      v637 = (*v15)-- == 1;
                      if ( v637 )
                      {
                        v7 = (sem_t *)v15;
                        (*(void (__fastcall **)(__int64 *))(v15[1] + 48))(v15);
                      }
                      goto LABEL_821;
                    }
                  }
                }
                else
                {
                  v49 = v40;
                  v7 = (sem_t *)v34;
                  LODWORD(v50) = PyCFunction_Call(v34, v40, 0LL);
                  v15 = v50;
                }
LABEL_46:
                if ( v40 )
                {
                  v637 = (*(_QWORD *)v40)-- == 1LL;
                  if ( v637 )
                  {
                    v7 = (sem_t *)v40;
                    (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v40 + 8) + 48LL))(v40, v49);
                    v51 = v922;
                    goto LABEL_49;
                  }
                }
                goto LABEL_93;
              }
              v135 = *(int (__fastcall **)(sem_t *, __int64))(v36 + 8);
              v136 = *(sem_t **)(v34 + 24);
              if ( v38 & 4 && !v29 )
              {
                if ( *((_DWORD *)PyThreadState_Current + 8)
                  && (v7 = (sem_t *)*((_QWORD *)PyThreadState_Current + 5)) != 0LL )
                {
                  if ( sub_5B63D0(
                         v7,
                         *((_QWORD *)PyThreadState_Current + 7),
                         *((_QWORD *)PyThreadState_Current + 2),
                         4LL,
                         v34) )
                  {
                    goto LABEL_92;
                  }
                  LODWORD(v71) = v135(v136, 0LL);
                  v7 = (sem_t *)*((_QWORD *)v37 + 5);
                  v15 = v71;
                  if ( !v7 )
                    goto LABEL_93;
                  v72 = *((_QWORD *)v37 + 2);
                  v73 = *((_QWORD *)v37 + 7);
                  if ( v71 )
                  {
                    if ( sub_5B63D0(v7, v73, v72, 6LL, v34) )
                    {
                      v637 = (*v15)-- == 1;
                      if ( v637 )
                      {
                        v7 = (sem_t *)v15;
                        (*(void (__fastcall **)(__int64 *))(v15[1] + 48))(v15);
                      }
LABEL_92:
                      v15 = 0LL;
                    }
LABEL_93:
                    v51 = v922;
                    goto LABEL_49;
                  }
                  sub_563E80(v7, v73, v72, 5LL, v34);
                  v51 = v922;
                }
                else
                {
                  v7 = *(sem_t **)(v34 + 24);
                  LODWORD(v216) = v135(v7, 0LL);
                  v51 = v922;
                  v15 = v216;
                }
LABEL_49:
                while ( 1 )
                {
                  v52 = (_QWORD *)v1059;
                  if ( v51 >= v1059 )
                    break;
                  while ( 1 )
                  {
                    v9 = (signed __int64)(v52 - 1);
                    v1059 = (__int64)(v52 - 1);
                    v7 = (sem_t *)*(v52 - 1);
                    v637 = v7->__align-- == 1;
                    if ( !v637 )
                      break;
                    (*(void (**)(void))(*(&v7->__align + 1) + 48))();
                    v52 = (_QWORD *)v1059;
                    if ( v51 >= v1059 )
                      goto LABEL_52;
                  }
                }
LABEL_52:
                v16 = (__int64)(v52 + 1);
                *v52 = v15;
                if ( v15 )
                  goto LABEL_19;
                goto LABEL_364;
              }
              if ( !(v38 & 8) || v29 != 1 )
              {
                if ( v38 & 4 )
                  v784 = "%.200s() takes no arguments (%d given)";
                else
                  v784 = "%.200s() takes exactly one argument (%d given)";
                v7 = (sem_t *)PyExc_TypeError;
                PyErr_Format(PyExc_TypeError, v784, *(_QWORD *)v36, (unsigned int)v29);
                goto LABEL_92;
              }
              v1059 = v16 - 8;
              v137 = *(_QWORD *)(v16 - 8);
              if ( *((_DWORD *)PyThreadState_Current + 8)
                && (v7 = (sem_t *)*((_QWORD *)PyThreadState_Current + 5)) != 0LL )
              {
                if ( sub_5B63D0(
                       v7,
                       *((_QWORD *)PyThreadState_Current + 7),
                       *((_QWORD *)PyThreadState_Current + 2),
                       4LL,
                       v34) )
                {
LABEL_1385:
                  v15 = 0LL;
                  goto LABEL_216;
                }
                LODWORD(v754) = v135(v136, v137);
                v7 = (sem_t *)*((_QWORD *)v37 + 5);
                v15 = v754;
                if ( v7 )
                {
                  v755 = *((_QWORD *)v37 + 2);
                  v756 = *((_QWORD *)v37 + 7);
                  if ( !v754 )
                  {
                    sub_563E80(v7, v756, v755, 5LL, v34);
                    goto LABEL_216;
                  }
                  if ( sub_5B63D0(v7, v756, v755, 6LL, v34) )
                  {
                    v637 = (*v15)-- == 1;
                    if ( v637 )
                    {
                      v7 = (sem_t *)v15;
                      (*(void (__fastcall **)(__int64 *))(v15[1] + 48))(v15);
                    }
                    goto LABEL_1385;
                  }
                }
              }
              else
              {
                v7 = v136;
                LODWORD(v138) = v135(v136, v137);
                v15 = v138;
              }
LABEL_216:
              v637 = (*(_QWORD *)v137)-- == 1LL;
              if ( !v637 )
                goto LABEL_93;
              v7 = (sem_t *)v137;
              (*(void (__fastcall **)(__int64))(*(_QWORD *)(v137 + 8) + 48LL))(v137);
              v51 = v922;
              goto LABEL_49;
            }
          }
          else if ( v35 == &PyMethod_Type )
          {
            v74 = *(_QWORD **)(v34 + 24);
            if ( v74 )
            {
              ++*v74;
              v34 = *(_QWORD *)(v34 + 16);
              ++*(_QWORD *)v34;
              v637 = (**(_QWORD **)v922)-- == 1LL;
              if ( v637 )
              {
                v75 = v74;
                (*(void (**)(void))(*(_QWORD *)(*(_QWORD *)v922 + 8LL) + 48LL))();
                v74 = v75;
              }
              ++v29;
              ++v32;
              *(_QWORD *)v922 = v74;
              if ( *(_UNKNOWN **)(v34 + 8) != &PyFunction_Type )
              {
LABEL_106:
                v78 = 0LL;
                if ( v31
                  && (v7 = 0LL, LODWORD(v426) = sub_488BF0(0LL, (unsigned int)v31, &v1059, v34), (v78 = v426) == 0)
                  || (v7 = (sem_t *)v29, LODWORD(v79) = PyTuple_New(v29), (v80 = v79) == 0) )
                {
                  v15 = 0LL;
LABEL_120:
                  if ( v78 )
                  {
                    v637 = (*(_QWORD *)v78)-- == 1LL;
                    if ( v637 )
                    {
                      v7 = (sem_t *)v78;
                      (*(void (__fastcall **)(__int64))(*(_QWORD *)(v78 + 8) + 48LL))(v78);
                    }
                  }
LABEL_101:
                  v637 = (*(_QWORD *)v34)-- == 1LL;
                  if ( !v637 )
                    goto LABEL_93;
                  v7 = (sem_t *)v34;
                  (*(void (__fastcall **)(__int64))(*(_QWORD *)(v34 + 8) + 48LL))(v34);
                  v51 = v922;
                  goto LABEL_49;
                }
                if ( v29 )
                {
                  v81 = v1059;
                  v4 = v1059 - 16;
                  v1059 -= 8LL;
                  *(_QWORD *)(v80 + 8LL * (v29 - 1) + 24) = *(_QWORD *)(v81 - 8);
                  if ( v29 != 1 )
                  {
                    v1059 = v4;
                    *(_QWORD *)(v80 + 8LL * (v29 - 2) + 24) = *(_QWORD *)(v81 - 16);
                    if ( v29 != 2 )
                    {
                      v1059 = v81 - 24;
                      v82 = v29 - 4;
                      *(_QWORD *)(v80 + 8LL * (v29 - 3) + 24) = *(_QWORD *)(v81 - 24);
                      if ( v29 != 3 )
                      {
                        v1059 = v81 - 32;
                        v4 = *(_QWORD *)(v81 - 32);
                        v83 = v29 - 5;
                        v84 = (__int64 *)(v81 - 40);
                        for ( *(_QWORD *)(v80 + 8LL * v82 + 24) = v4; v83 != -1; *(_QWORD *)(v80 + 8 * v86 + 24) = v85 )
                        {
                          v1059 = (__int64)v84;
                          v85 = *v84;
                          v86 = v83--;
                          --v84;
                        }
                      }
                    }
                  }
                }
                v87 = *(_QWORD *)(v34 + 8);
                if ( (_UNKNOWN *)v87 == &PyCFunction_Type )
                {
                  v543 = PyThreadState_Current;
                  if ( !*((_DWORD *)PyThreadState_Current + 8)
                    || (v7 = (sem_t *)*((_QWORD *)PyThreadState_Current + 5)) == 0LL )
                  {
                    v7 = (sem_t *)v34;
                    LODWORD(v544) = PyCFunction_Call(v34, v80, v78);
                    v15 = v544;
                    goto LABEL_118;
                  }
                  if ( sub_5B63D0(
                         v7,
                         *((_QWORD *)PyThreadState_Current + 7),
                         *((_QWORD *)PyThreadState_Current + 2),
                         4LL,
                         v34) )
                  {
                    goto LABEL_942;
                  }
                  LODWORD(v864) = PyCFunction_Call(v34, v80, v78);
                  v7 = (sem_t *)*((_QWORD *)v543 + 5);
                  v15 = v864;
                  if ( v7 )
                  {
                    v865 = *((_QWORD *)v543 + 2);
                    v866 = *((_QWORD *)v543 + 7);
                    if ( v864 )
                    {
                      if ( sub_5B63D0(v7, v866, v865, 6LL, v34) )
                      {
                        v637 = (*v15)-- == 1;
                        if ( !v637 )
                        {
LABEL_942:
                          v15 = 0LL;
                          goto LABEL_118;
                        }
                        v867 = v15[1];
                        v7 = (sem_t *)v15;
                        v15 = 0LL;
                        (*(void (__fastcall **)(sem_t *))(v867 + 48))(v7);
                      }
                    }
                    else
                    {
                      sub_563E80(v7, v866, v865, 5LL, v34);
                    }
                  }
                }
                else
                {
                  v88 = *(int (__fastcall **)(__int64, __int64, __int64, signed __int64))(v87 + 128);
                  if ( v88 )
                  {
                    v89 = *((_DWORD *)PyThreadState_Current + 6) + 1;
                    v639 = __OFSUB__(v89, Py_CheckRecursionLimit);
                    v637 = v89 == Py_CheckRecursionLimit;
                    v638 = v89 - Py_CheckRecursionLimit < 0;
                    *((_DWORD *)PyThreadState_Current + 6) = v89;
                    if ( !((unsigned __int8)(v638 ^ v639) | v637) )
                    {
                      v7 = (sem_t *)" while calling a Python object";
                      if ( Py_CheckRecursiveCall(" while calling a Python object") )
                        goto LABEL_942;
                    }
                    v7 = (sem_t *)v34;
                    LODWORD(v90) = v88(v34, v80, v78, v4);
                    v10 = (signed __int64)PyThreadState_Current;
                    v15 = v90;
                    --*(_DWORD *)(v10 + 24);
                    if ( v90 )
                      goto LABEL_118;
                    if ( *(_QWORD *)(v10 + 72) )
                      goto LABEL_942;
                    v7 = (sem_t *)PyExc_SystemError;
                    PyErr_SetString(PyExc_SystemError, "NULL result without error in PyObject_Call");
                  }
                  else
                  {
                    v7 = (sem_t *)PyExc_TypeError;
                    v15 = 0LL;
                    PyErr_Format(PyExc_TypeError, "'%.200s' object is not callable", *(_QWORD *)(v87 + 24), v4);
                  }
                }
LABEL_118:
                v637 = (*(_QWORD *)v80)-- == 1LL;
                if ( v637 )
                {
                  v7 = (sem_t *)v80;
                  (*(void (__fastcall **)(__int64))(*(_QWORD *)(v80 + 8) + 48LL))(v80);
                }
                goto LABEL_120;
              }
LABEL_99:
              v76 = *(_QWORD *)(v34 + 32);
              v7 = *(sem_t **)(v34 + 16);
              v77 = *(_QWORD *)(v34 + 24);
              if ( v76 )
              {
                v15 = (__int64 *)PyEval_EvalCodeEx(
                                   (signed __int64)v7,
                                   v77,
                                   0LL,
                                   v1059 - 8LL * v32,
                                   v29,
                                   v1059 - 8LL * v30,
                                   v31,
                                   v76 + 24,
                                   *(_QWORD *)(v76 + 16),
                                   *(_QWORD *)(v34 + 40));
              }
              else if ( v32 != *((_DWORD *)&v7->__align + 4) || v31 || *((_DWORD *)&v7->__align + 7) != 67 )
              {
                v15 = (__int64 *)PyEval_EvalCodeEx(
                                   (signed __int64)v7,
                                   v77,
                                   0LL,
                                   v1059 - 8LL * v32,
                                   v29,LABEL_194:
                                   v1059 - 8LL * v30,
                                   v31,
                                   0LL,
                                   0,
                                   *(_QWORD *)(v34 + 40));
              }
              else
              {
                v98 = PyThreadState_Current;
                v7 = (sem_t *)PyThreadState_Current;
                v99 = PyFrame_New(
                        (signed __int64)PyThreadState_Current,
                        *(const char **)(v34 + 16),
                        *(_QWORD **)(v34 + 24),
                        0LL);
                v100 = v99;
                if ( v99 )
                {
                  if ( v32 > 0 )
                  {
                    v101 = v1059 - 8LL * v32;
                    ++**(_QWORD **)v101;
                    *(_QWORD *)(v99 + 376) = *(_QWORD *)v101;
                    if ( v32 > 1 )
                    {
                      ++**(_QWORD **)(v101 + 8);
                      *(_QWORD *)(v99 + 384) = *(_QWORD *)(v101 + 8);
                      if ( v32 > 2 )
                      {
                        ++**(_QWORD **)(v101 + 16);
                        *(_QWORD *)(v99 + 392) = *(_QWORD *)(v101 + 16);
                        if ( v32 > 3 )
                        {
                          ++**(_QWORD **)(v101 + 24);
                          *(_QWORD *)(v99 + 400) = *(_QWORD *)(v101 + 24);
                          if ( v32 > 4 )
                          {
                            v102 = 5LL;
                            ++**(_QWORD **)(v101 + 32);
                            *(_QWORD *)(v99 + 408) = *(_QWORD *)(v101 + 32);
                            if ( v32 > 5 )
                            {
                              do
                              {
                                ++**(_QWORD **)(v101 + 8 * v102);
                                *(_QWORD *)(v99 + 8 * v102 + 376) = *(_QWORD *)(v101 + 8 * v102);
                                ++v102;
                              }
                              while ( v32 > (signed int)v102 );
                            }
                          }
                        }
                      }
                    }
                  }
                  v7 = (sem_t *)v99;
                  v103 = PyEval_EvalFrameEx(v99, 0LL);
                  ++v98[6];
                  v15 = (__int64 *)v103;
                  v637 = (*(_QWORD *)v100)-- == 1LL;
                  if ( v637 )
                  {
                    v7 = (sem_t *)v100;
                    (*(void (__fastcall **)(signed __int64, _QWORD))(*(_QWORD *)(v100 + 8) + 48LL))(v100, 0LL);
                  }
                  --v98[6];
                }
                else
                {
                  v15 = 0LL;
                }
              }
              goto LABEL_101;
            }
          }
          ++*(_QWORD *)v34;
          if ( v35 != &PyFunction_Type )
            goto LABEL_106;
          goto LABEL_99;
        case 100:
        case 201:
          v15 = *(__int64 **)(v1009 + 8LL * v21 + 24);
LABEL_16:
          ++*v15;
          next_instr = v20;
          *(_QWORD *)v16 = v15;
          v16 += 8LL;
          continue;
        case 125:
          v10 = v919;
          v53 = *(_QWORD *)(v16 - 8);
          v16 -= 8LL;
          v4 = v919 + 8LL * v21;
          next_instr = v20;
          v7 = *(sem_t **)v4;
          *(_QWORD *)v4 = v53;
          if ( v7 )
            goto LABEL_56;
          continue;
        case 116:
        case 202:
          v14 = (sem_t *)v16;
          v55 = *(_QWORD *)(v1004 + 8LL * v21 + 24);
          if ( *(_UNKNOWN **)(v55 + 8) != &PyString_Type || (v56 = *(_QWORD *)(v55 + 24), v56 == -1) )
          {
            v1002 = v16;
            v7 = *(sem_t **)(v914 + 48);
            LODWORD(v846) = PyDict_GetItem(v7, *(_QWORD *)(v1004 + 8LL * v21 + 24));
            v15 = v846;
            v16 = v1002;
            if ( !v846 )
            {
              v7 = *(sem_t **)(v914 + 40);
              LODWORD(v847) = PyDict_GetItem(v7, v55);
              v15 = v847;
              v16 = v1002;
              if ( !v847 )
              {
LABEL_1569:
                v7 = (sem_t *)PyExc_NameError;
                v848 = v16;
                v15 = 0LL;
                sub_44DD3B(PyExc_NameError, "global name '%.200s' is not defined", v55);
                v16 = v848;
                goto LABEL_364;
              }
            }
            goto LABEL_505;
          }
          v923 = v16;
          v7 = *(sem_t **)(v914 + 48);
          LODWORD(v57) = (*((int (__fastcall **)(sem_t *, _QWORD, _QWORD))&v7[1].__align + 2))(v7, v55, v56);
          v16 = v923;
          if ( !v57 )
          {
LABEL_1543:
            v15 = 0LL;
            goto LABEL_364;
          }
          v15 = *(__int64 **)(v57 + 16);
          if ( v15 )
            goto LABEL_64;
          v7 = *(sem_t **)(v914 + 40);
          LODWORD(v58) = (*((int (__fastcall **)(_QWORD, _QWORD, _QWORD))&v7[1].__align + 2))(v7, v55, v56);
          v16 = v923;
          if ( v58 )
          {
            v15 = *(__int64 **)(v58 + 16);
            if ( v15 )
            {
LABEL_64:
              ++*v15;
              v16 += 8LL;
              v14->__align = (__int64)v15;
              next_instr = v20;
              goto LABEL_10;
            }
            goto LABEL_1569;
          }
LABEL_364:
          a2 = (const char *)(unsigned int)v1005;
          LOBYTE(v7) = v15 != 0LL;
          if ( v1005 == 0 && v15 != 0LL )
          {
            v1005 = 0;
            next_instr = v20;
            goto LABEL_10;
          }
          v11 = v16;
          v817 = 0;
          v818 = &Py_NoneStruct;
          PyFrameObject = v914;
          v1005 = 0;
          v304 = &Py_NoneStruct;
          if ( *((_QWORD *)PyThreadState_Current + 9) )
          {
            v7 = (sem_t *)v16;
          }
          else
          {
LABEL_1517:
            a2 = "error return without exception set";
            PyErr_SetString(PyExc_SystemError, "error return without exception set");
            v304 = v818;
            v1005 = v817;
            v7 = (sem_t *)v11;
          }
          goto LABEL_500;
        case 107:
          v59 = *(_QWORD *)(v16 - 8);
          v60 = v16 - 8;
          v61 = *(_QWORD *)(v16 - 16);
          v10 = *(_QWORD *)(v59 + 8);
          if ( (_UNKNOWN *)v10 != &PyInt_Type || *(_UNKNOWN **)(v61 + 8) != &PyInt_Type )
          {
LABEL_66:
            switch ( v21 )
            {
              case 6:
                if ( *(_BYTE *)(v10 + 168) & 2
                  && (v125 = *(_QWORD *)(v10 + 104)) != 0
                  && (v126 = *(int (__fastcall **)(__int64, __int64))(v125 + 56)) != 0LL )
                {
                  v127 = v16;
                  a2 = *(const char **)(v16 - 16);
                  v7 = *(sem_t **)(v16 - 8);
                  v128 = v126(v59, v61);
                  v16 = v127;
                }
                else
                {
                  a2 = *(const char **)(v16 - 16);
                  v7 = *(sem_t **)(v16 - 8);
                  v690 = v16;
                  v128 = PySequence_IterSearch(v7, a2, 3LL);
                  v16 = v690;
                }
                if ( v128 >= 0 )
                  goto LABEL_191;
                goto LABEL_1446;
              case 9:
                v128 = v59 != v61;
                goto LABEL_191;
              case 8:
                v128 = v59 == v61;
                goto LABEL_191;
              case 7:
                if ( *(_BYTE *)(v10 + 168) & 2
                  && (v229 = *(_QWORD *)(v10 + 104)) != 0
                  && (v230 = *(int (__fastcall **)(__int64, __int64))(v229 + 56)) != 0LL )
                {
                  v231 = v16;
                  a2 = *(const char **)(v16 - 16);
                  v7 = *(sem_t **)(v16 - 8);
                  v232 = v230(v59, v61);
                  v16 = v231;
                }
                else
                {
                  a2 = *(const char **)(v16 - 16);
                  v7 = *(sem_t **)(v16 - 8);
                  v824 = v16;
                  v232 = PySequence_IterSearch(v7, a2, 3LL);
                  v16 = v824;
                }
                if ( v232 < 0 )
                  goto LABEL_1446;
                v128 = v232 == 0;
                goto LABEL_191;
              case 10:
                v316 = *(_QWORD *)(v10 + 168);
                if ( v316 & 0x4000000 )
                {
                  v655 = *(_QWORD *)(v59 + 16);
                  if ( v655 > 0 )
                  {
                    v656 = *(_QWORD *)(v16 - 16);
                    v985 = v16;
                    v657 = v20;
                    v658 = 0LL;
                    do
                    {
                      v659 = *(_QWORD *)(v59 + 8 * v658 + 24);
                      v660 = *(_QWORD *)(*(_QWORD *)(v659 + 8) + 168LL);
                      if ( v660 & 0x8000000 )
                      {
                        v7 = (sem_t *)PyExc_DeprecationWarning;
                        a2 = "catching of string exceptions is deprecated";
                        if ( PyErr_WarnEx(PyExc_DeprecationWarning, "catching of string exceptions is deprecated", 1LL) < 0 )
                          goto LABEL_1445;
                      }
                      else if ( Py_Py3kWarningFlag
                             && !(v660 & 0x4000000)
                             && (!(v660 & 0x80000000) || !(*(_BYTE *)(v659 + 171) & 0x40)) )
                      {
                        v7 = (sem_t *)PyExc_DeprecationWarning;
                        a2 = "catching classes that don't inherit from BaseException is not allowed in 3.x";
                        if ( PyErr_WarnEx(
                               PyExc_DeprecationWarning,
                               "catching classes that don't inherit from BaseException is not allowed in 3.x",
                               1LL) < 0 )
                        {
LABEL_1445:
                          v16 = v985;
                          v20 = v657;
                          v61 = v656;
                          goto LABEL_1446;
                        }
                      }
                      ++v658;
                    }
                    while ( v658 != v655 );
                    v20 = v657;
                    v16 = v985;
                    v61 = v656;
                  }
LABEL_521:
                  a2 = (const char *)v59;
                  v7 = (sem_t *)v61;
                  v317 = v16;
                  v128 = PyErr_GivenExceptionMatches(v61, v59);
                  v16 = v317;
                  goto LABEL_191;
                }
                if ( v316 & 0x8000000 )
                {
                  v1003 = v16;
                  a2 = "catching of string exceptions is deprecated";
                }
                else
                {
                  if ( !Py_Py3kWarningFlag || v316 & 0x80000000 && *(_BYTE *)(v59 + 171) & 0x40 )
                    goto LABEL_521;
                  v1003 = v16;
                  a2 = "catching classes that don't inherit from BaseException is not allowed in 3.x";
                }
                v7 = (sem_t *)PyExc_DeprecationWarning;
                v854 = PyErr_WarnEx(PyExc_DeprecationWarning, a2, 1LL);
                v16 = v1003;
                if ( v854 >= 0 )
                  goto LABEL_521;
LABEL_1446:
                v15 = 0LL;
                goto LABEL_194;
              default:
                v139 = *((_DWORD *)PyThreadState_Current + 6) + 1;
                v639 = __OFSUB__(v139, Py_CheckRecursionLimit);
                v637 = v139 == Py_CheckRecursionLimit;
                v638 = v139 - Py_CheckRecursionLimit < 0;
                *((_DWORD *)PyThreadState_Current + 6) = v139;
                if ( !((unsigned __int8)(v638 ^ v639) | v637) )
                {
                  v7 = (sem_t *)" in cmp";
                  v829 = v16;
                  v15 = 0LL;
                  v830 = Py_CheckRecursiveCall(" in cmp");
                  v16 = v829;
                  if ( v830 )
                    goto LABEL_194;
                }
                v140 = *(_QWORD *)(v61 + 8);
                if ( v140 != *(_QWORD *)(v59 + 8) || (_UNKNOWN *)v140 == &PyInstance_Type )
                  goto LABEL_712;
                if ( !(*(_BYTE *)(v140 + 168) & 0x20) )
                  goto LABEL_616;
                v141 = *(int (__fastcall **)(__int64, __int64, _QWORD))(v140 + 200);
                if ( !v141 )
                  goto LABEL_616;
                v927 = v16;
                a2 = (const char *)v59;
                LODWORD(v142) = v141(v61, v59, (unsigned int)v21);
                v16 = v927;
                if ( v142 != &Py_NotImplementedStruct )
                {
                  v7 = (sem_t *)PyThreadState_Current;
                  goto LABEL_225;
                }
                if ( --Py_NotImplementedStruct )
                {
                  v140 = *(_QWORD *)(v61 + 8);
                }
                else
                {
                  (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
                  v140 = *(_QWORD *)(v61 + 8);
                  v16 = v927;
                }
LABEL_616:
                v384 = *(int (__fastcall **)(__int64, __int64))(v140 + 80);
                if ( !v384 )
                {
LABEL_712:
                  a2 = (const char *)v59;
                  v427 = v16;
                  LODWORD(v142) = sub_531820(v61, v59, (unsigned int)v21);
                  v7 = (sem_t *)PyThreadState_Current;
                  v16 = v427;
                  goto LABEL_225;
                }
                v965 = v16;
                a2 = (const char *)v59;
                v385 = v384(v61, v59);
                v7 = (sem_t *)PyThreadState_Current;
                v386 = v385;
                v16 = v965;
                if ( *((_QWORD *)PyThreadState_Current + 9) )
                {
                  v812 = sub_45F608((unsigned int)v385);
                  v7 = (sem_t *)PyThreadState_Current;
                  v387 = v812;
                  v16 = v965;
LABEL_620:
                  if ( v387 == -2 )
                  {
                    v142 = 0LL;
                    goto LABEL_225;
                  }
                  goto LABEL_621;
                }
                a2 = (const char *)(unsigned int)(v385 + 1);
                if ( (unsigned int)a2 <= 2 )
                {
                  v387 = v385;
                  goto LABEL_620;
                }
                a2 = "tp_compare didn't return -1, 0 or 1";
                v881 = PyErr_WarnEx(PyExc_RuntimeWarning, "tp_compare didn't return -1, 0 or 1", 1LL);
                v16 = v965;
                if ( v881 < 0 )
                {
                  v7 = (sem_t *)PyThreadState_Current;
                  v142 = 0LL;
                  goto LABEL_225;
                }
                v7 = (sem_t *)PyThreadState_Current;
                v4 = !((v386 + 1 < 0) ^ __OFADD__(1, v386));
                v387 = 2 * v4 - 1;
LABEL_621:
                switch ( v21 )
                {
                  case 2:
                    v387 = v387 == 0;
                    break;
                  case 5:
                    v387 = ~v387;
                    goto LABEL_627;
                  case 0:
LABEL_627:
                    v387 >>= 31;
                    break;
                  case 4:
                    LOBYTE(v9) = (signed int)v387 > 0;
                    v387 = (signed int)v387 > 0;
                    break;
                  case 3:
                    v387 = v387 != 0;
                    break;
                  case 1:
                    v387 = (signed int)v387 <= 0;
                    break;
                  default:
                    break;
                }
                v142 = &Py_TrueStruct;
                if ( !v387 )
                  v142 = &Py_ZeroStruct;
                ++*v142;
LABEL_225:
                --*((_DWORD *)&v7->__align + 6);
                v15 = v142;
                v637 = (*(_QWORD *)v61)-- == 1LL;
                if ( v637 )
                  goto LABEL_226;
                break;
            }
LABEL_195:
            v637 = (*(_QWORD *)v59)-- == 1LL;
            if ( !v637 )
              goto LABEL_196;
            goto LABEL_227;
          }
          v7 = *(sem_t **)(v61 + 16);
          a2 = *(const char **)(v59 + 16);
          v4 = (unsigned int)v21;
          switch ( v21 )
          {
            default:
              goto LABEL_66;
            case 2:
              v128 = (const char *)v7 == a2;
              break;
            case 3:
              v128 = (const char *)v7 != a2;
              break;
            case 5:
              v128 = (signed __int64)v7 < (signed __int64)a2;
              LOBYTE(v128) = (signed __int64)v7 >= (signed __int64)a2;
              break;
            case 0:
              v128 = (signed __int64)v7 < (signed __int64)a2;
              break;
            case 4:
              v128 = (signed __int64)v7 < (signed __int64)a2;
              LOBYTE(v128) = (signed __int64)v7 > (signed __int64)a2;
              break;
            case 1:
              v128 = (signed __int64)v7 < (signed __int64)a2;
              LOBYTE(v128) = (signed __int64)v7 <= (signed __int64)a2;
              break;
            case 8:
              v128 = v61 == v59;
              break;
            case 9:
              v128 = v61 != v59;
              break;
          }
LABEL_191:
          v15 = &Py_TrueStruct;
          v9 = (signed __int64)&Py_ZeroStruct;
          if ( !v128 )
            v15 = &Py_ZeroStruct;
          ++*v15;
LABEL_194:
          v637 = (*(_QWORD *)v61)-- == 1LL;
          if ( !v637 )
            goto LABEL_195;
LABEL_226:
          v143 = v16;
          v7 = (sem_t *)v61;
          (*(void (__fastcall **)(__int64))(*(_QWORD *)(v61 + 8) + 48LL))(v61);
          v637 = (*(_QWORD *)v59)-- == 1LL;
          v16 = v143;
          if ( !v637 )
            goto LABEL_196;
LABEL_227:
          v144 = v16;
          v7 = (sem_t *)v59;
          (*(void (__fastcall **)(__int64))(*(_QWORD *)(v59 + 8) + 48LL))(v59);
          v16 = v144;
LABEL_196:
          *(_QWORD *)(v16 - 16) = v15;
          if ( !v15 )
          {
LABEL_1054:
            v16 = v60;
            goto LABEL_364;
          }
          if ( *(_BYTE *)v20 == 114 )
          {
            v97 = v15;
            next_instr = v20 + 3;
            v16 = v60 - 8;
            v21 = (*(_BYTE *)(v20 + 2) << 8) + *(_BYTE *)(v20 + 1);
            if ( v15 == &Py_TrueStruct )
            {
LABEL_199:
              if ( !--Py_TrueStruct )
              {
                v129 = v16;
                v7 = (sem_t *)&Py_TrueStruct;
                (*((void (__fastcall **)(_QWORD))off_920738 + 6))(&Py_TrueStruct);
                v16 = v129;
              }
              continue;
            }
LABEL_143:
            if ( v97 == &Py_ZeroStruct )
            {
              if ( !--Py_ZeroStruct )
              {
                v831 = v16;
                v7 = (sem_t *)&Py_ZeroStruct;
                (*((void (__fastcall **)(_QWORD))off_920588 + 6))(&Py_ZeroStruct);
                v16 = v831;
              }
              next_instr = v918 + v21;
              continue;
            }
            v1005 = 0;
            if ( v97 == &Py_NoneStruct )
            {
LABEL_340:
              v637 = (*v97)-- == 1;
              if ( v637 )
              {
                v502 = v16;
                v7 = (sem_t *)v97;
                (*(void (__fastcall **)(__int64 *))(v97[1] + 48))(v97);
                v16 = v502;
              }
              if ( v1005 > 0 )
              {
                v20 = next_instr;
                v1005 = 0;
              }
              else
              {
                if ( v1005 )
                {
LABEL_652:
                  v20 = next_instr;
                  goto LABEL_364;
                }
LABEL_344:
                v4 = v918;
                v20 = v918 + v21;
              }
              goto LABEL_19;
            }
            v10 = v97[1];
            v380 = *(_QWORD *)(v10 + 96);
            if ( v380 && (v381 = *(int (__fastcall **)(__int64 *))(v380 + 80)) != 0LL )
            {
              v382 = v16;
              v7 = (sem_t *)v97;
              LODWORD(v383) = v381(v97);
              v16 = v382;
              v383 = (signed int)v383;
LABEL_612:
              if ( v383 <= 0 )
              {
                v1005 = v383;
                goto LABEL_340;
              }
            }
            else
            {
              v7 = *(sem_t **)(v10 + 112);
              if ( v7 )
              {
                v388 = (int (__fastcall *)(__int64 *))v7->__align;
                if ( v7->__align )
                {
                  v389 = v16;
                  v7 = (sem_t *)v97;
                  LODWORD(v383) = v388(v97);
                  v16 = v389;
                  goto LABEL_612;
                }
              }
              v508 = *(int (__fastcall ***)(_QWORD))(v10 + 104);
              if ( v508 && *v508 )
              {
                v509 = v16;
                v7 = (sem_t *)v97;
                LODWORD(v383) = (*v508)(v97);
                v16 = v509;
                goto LABEL_612;
              }
            }
            v1005 = 1;
            goto LABEL_340;
          }
          if ( *(_BYTE *)v20 != 115 )
          {
LABEL_229:
            v16 = v60;
            goto LABEL_19;
          }
          v154 = *(_BYTE *)(v20 + 2);
          v155 = *(_BYTE *)(v20 + 1);
          v114 = v15;
          v20 += 3LL;
          v16 = v60;
          v7 = (sem_t *)(unsigned int)(v154 << 8);
          v21 = (_DWORD)v7 + v155;
LABEL_175:
          v16 -= 8LL;
          if ( v114 != &Py_ZeroStruct )
          {
            if ( v114 == &Py_TrueStruct )
            {
              if ( !--Py_TrueStruct )
              {
                v832 = v16;
                v7 = (sem_t *)&Py_TrueStruct;
                (*((void (__fastcall **)(_QWORD))off_920738 + 6))(&Py_TrueStruct);
                v16 = v832;
              }
LABEL_179:
              v9 = v918;
              next_instr = v918 + v21;
              continue;
            }
            v7 = (sem_t *)v114;
            v934 = v16;
            v220 = PyObject_IsTrue(v114, a2);
            v637 = (*v114)-- == 1;
            v1005 = v220;
            v16 = v934;
            if ( v637 )
            {
              v7 = (sem_t *)v114;
              (*(void (__fastcall **)(__int64 *))(v114[1] + 48))(v114);
              v16 = v934;
            }
            if ( v1005 > 0 )
              goto LABEL_356;
            if ( !v1005 )
              goto LABEL_19;
            goto LABEL_364;
          }
          goto LABEL_359;
        case 83:
          v62 = v15;
          PyFrameObject = v914;
          v7 = (sem_t *)(v16 - 8);
          v1010 = *(_QWORD *)(v16 - 8);
          v63 = 8;
          goto LABEL_68;
        case 1:
          v7 = *(sem_t **)(v16 - 8);
          v16 -= 8LL;
          next_instr = v20;
          v637 = v7->__align-- == 1;
          if ( v637 )
            goto LABEL_57;
          continue;
        case 93:
          v7 = *(sem_t **)(v16 - 8);
          goto LABEL_80;
        case 94:
          v91 = *(_QWORD *)(v16 - 8);
          v925 = v16 - 8;
          v92 = PyList_Append(*(_QWORD *)(v16 + 8LL * -v21 - 8), *(_QWORD *)(v16 - 8));
          goto LABEL_124;
        case 113:
          goto LABEL_128;
        case 25:
          v93 = v16;
          v16 -= 8LL;
          v94 = *(_QWORD *)(v93 - 16);
          v95 = *(_QWORD *)(v93 - 8);
          if ( *(_UNKNOWN **)(v94 + 8) != &PyList_Type || *(_UNKNOWN **)(v95 + 8) != &PyInt_Type )
            goto LABEL_338;
          if ( qword_9172A8 & 0x800000 )
          {
            v96 = *(_QWORD *)(v95 + 16);
          }
          else
          {
            v993 = v16;
            v7 = *(sem_t **)(v93 - 8);
            if ( qword_9172A8 & 0x1000000 )
            {
              LODWORD(v96) = PyLong_AsSsize_t(v95);
              v16 = v993;
            }
            else
            {
              LODWORD(v96) = PyInt_AsLong(v95, a2);
              v16 = v993;
            }
          }
          if ( v96 < 0 )
          {
            v7 = *(sem_t **)(v94 + 16);
            v96 += (signed __int64)v7;
            if ( v96 >= 0 && (signed __int64)v7 > v96 )
            {
LABEL_135:
              v15 = *(__int64 **)(*(_QWORD *)(v94 + 24) + 8 * v96);
              ++*v15;
              goto LABEL_136;
            }
          }
          else if ( v96 < *(_QWORD *)(v94 + 16) )
          {
            goto LABEL_135;
          }
LABEL_338:
          v7 = (sem_t *)v94;
          v217 = v16;
          LODWORD(v218) = PyObject_GetItem(v94, v95);
          v16 = v217;
          v15 = v218;
LABEL_136:
          v637 = (*(_QWORD *)v94)-- == 1LL;
          if ( v637 )
          {
            v392 = v16;
            v7 = (sem_t *)v94;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v94 + 8) + 48LL))(v94);
            v16 = v392;
          }
          v637 = (*(_QWORD *)v95)-- == 1LL;
          if ( v637 )
          {
            v446 = v16;
            v7 = (sem_t *)v95;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v95 + 8) + 48LL))(v95);
            v16 = v446;
          }
LABEL_140:
          *(_QWORD *)(v93 - 16) = v15;
          if ( !v15 )
            goto LABEL_364;
          goto LABEL_19;
        case 114:
          v97 = *(__int64 **)(v16 - 8);
          next_instr = v20;
          v16 -= 8LL;
          if ( v97 != &Py_TrueStruct )
            goto LABEL_143;
          goto LABEL_199;
        case 92:
          v7 = *(sem_t **)(v16 - 8);
          goto LABEL_162;
        case 115:
          v114 = *(__int64 **)(v16 - 8);
          goto LABEL_175;
        case 23:
          v93 = v16;
          v115 = v16 - 8;
          v116 = *(_QWORD *)(v93 - 16);
          v117 = *(_QWORD *)(v93 - 8);
          v118 = *(void **)(v116 + 8);
          if ( v118 == &PyInt_Type )
          {
            if ( *(_UNKNOWN **)(v117 + 8) == &PyInt_Type )
            {
              v119 = *(_QWORD *)(v116 + 16);
              v120 = *(_QWORD *)(v117 + 16);
              v7 = (sem_t *)(v120 + v119);
              if ( ((v120 + v119) ^ v119) >= 0 || (((unsigned __int64)v7 ^ v120) & 0x8000000000000000LL) == 0LL )
              {
                v121 = v115;
                LODWORD(v122) = PyInt_FromLong(v7);
                v16 = v121;
                v15 = v122;
                goto LABEL_269;
              }
            }
          }
          else if ( v118 == &PyString_Type && *(_UNKNOWN **)(v117 + 8) == &PyString_Type )
          {
            v7 = *(sem_t **)(v93 - 16);
            v390 = v115;
            LODWORD(v391) = sub_558820(v7, *(_QWORD *)(v93 - 8), v914, v20);
            v16 = v390;
            v15 = v391;
            goto LABEL_271;
          }
          v7 = *(sem_t **)(v93 - 16);
          v930 = v115;
          LODWORD(v166) = sub_561E20(v7, *(_QWORD *)(v93 - 8), 0LL);
          v15 = v166;
          v16 = v930;
          if ( v166 == &Py_NotImplementedStruct )
          {
            v637 = Py_NotImplementedStruct-- == 1;
            v167 = *(_QWORD *)(*(_QWORD *)(v116 + 8) + 104LL);
            if ( v637 )
            {
              (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
              v16 = v930;
            }
            if ( v167 && (v168 = *(int (__fastcall **)(__int64, __int64))(v167 + 8)) != 0LL )
            {
              v169 = v16;
              v7 = (sem_t *)v116;
              LODWORD(v170) = v168(v116, v117);
              v16 = v169;
              v15 = v170;
            }
            else
            {
              v910 = v16;
              v7 = (sem_t *)PyExc_TypeError;
              v15 = 0LL;
              v911 = *(_QWORD *)(*(_QWORD *)(v117 + 8) + 24LL);
              PyErr_Format(
                PyExc_TypeError,
                "unsupported operand type(s) for %.100s: '%.100s' and '%.100s'",
                "+",
                *(_QWORD *)(*(_QWORD *)(v116 + 8) + 24LL));
              v16 = v910;
            }
          }
LABEL_269:
          v637 = (*(_QWORD *)v116)-- == 1LL;
          if ( v637 )
          {
            v221 = v16;
            v7 = (sem_t *)v116;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v116 + 8) + 48LL))(v116);
            v16 = v221;
          }
LABEL_271:
          v637 = (*(_QWORD *)v117)-- == 1LL;
          if ( v637 )
          {
            v171 = v16;
            v7 = (sem_t *)v117;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v117 + 8) + 48LL))(v117);
            v16 = v171;
          }
          goto LABEL_140;
        case 120:
        case 121:
        case 122:
          v123 = *(_DWORD *)(v914 + 128);
          v124 = v20 - v918 + v21;
          v9 = (v16 - *(_QWORD *)(v914 + 64)) >> 3;
          if ( (signed int)v123 > 19 )
            goto LABEL_1540;
          v10 = v914 + 12 * v123 + 128;
          *(_DWORD *)(v914 + 128) = v123 + 1;
          *(_DWORD *)(v10 + 4) = opcode;
          *(_DWORD *)(v10 + 12) = v9;
          *(_DWORD *)(v10 + 8) = v124;
          goto LABEL_19;
        case 102:
          v7 = (sem_t *)v21;
          v926 = v16;
          LODWORD(v130) = PyTuple_New(v21);
          v15 = v130;
          v16 = v926;
          if ( !v130 )
            goto LABEL_364;
          v10 = v926;
          v9 = (unsigned int)(v21 - 1);
          if ( v21 - 1 >= 0 )
          {
            v130[(signed int)v9 + 3] = *(_QWORD *)(v926 - 8);
            if ( v21 != 1 )
            {
              v130[v21 + 1] = *(_QWORD *)(v926 - 16);
              if ( v21 != 2 )
              {
                v7 = *(sem_t **)(v926 - 24);
                v130[v21] = (__int64)v7;
                if ( v21 != 3 )
                {
                  v131 = v21 - 5;
                  v130[v21 - 1] = *(_QWORD *)(v926 - 32);
                  if ( v21 != 4 )
                  {
                    v132 = v21 - 6;
                    v4 = v131;
                    v133 = (__int64 *)(v926 - 40);
                    for ( v130[v131 + 3] = *(_QWORD *)(v926 - 40); v132 != -1; v130[v134 + 3] = *v133 )
                    {
                      --v133;
                      v134 = v132--;
                    }
                  }
                }
              }
            }
            v10 = v926 + 8 * ~(unsigned __int64)(unsigned int)v9;
          }
          goto LABEL_210;
        case 95:
          v145 = *(_QWORD *)(v16 - 8);
          v146 = *(_QWORD *)(v16 - 16);
          v928 = v16 - 16;
          v147 = PyObject_SetAttr(*(_QWORD *)(v16 - 8), *(_QWORD *)(v1004 + 8LL * v21 + 24), *(_QWORD *)(v16 - 16));
          v637 = (*(_QWORD *)v145)-- == 1LL;
          v1005 = v147;
          v16 = v928;
          if ( v637 )
          {
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v145 + 8) + 48LL))(v145);
            v16 = v928;
          }
          goto LABEL_232;
        case 110:
          v10 = v21;
          next_instr = v20 + v21;
          continue;
        case 87:
          v148 = *(_DWORD *)(v914 + 128);
          if ( v148 <= 0 )
            Py_FatalError("XXX block stack underflow", a2);
          v7 = (sem_t *)v914;
          v149 = v148 - 1;
          *(_DWORD *)(v914 + 128) = v149;
          v150 = v914 + 12LL * v149 + 132;
          if ( *(_DWORD *)(v914 + 12LL * v149 + 140) >= (signed int)((v16 - *(_QWORD *)(v914 + 64)) >> 3) )
            goto LABEL_19;
          v151 = v16 - 8;
          v7 = *(sem_t **)(v16 - 8);
          v16 -= 8LL;
          v637 = v7->__align-- == 1;
          if ( v637 )
          {
            (*(void (**)(void))(*(&v7->__align + 1) + 48))();
            v16 = v151;
          }
          v10 = v914;
          v4 = (v151 - *(_QWORD *)(v914 + 64)) >> 3;
          if ( *(_DWORD *)(v150 + 8) >= (signed int)v4 )
            goto LABEL_19;
          v7 = *(sem_t **)(v151 - 8);
          v152 = v151 - 8;
          v637 = v7->__align-- == 1;
          if ( v637 )
          {
            (*(void (**)(void))(*(&v7->__align + 1) + 48))();
            v153 = (sem_t **)(v151 - 8);
          }
          else
          {
            v153 = (sem_t **)(v151 - 8);
          }
          while ( *(_DWORD *)(v150 + 8) < (signed int)(((signed __int64)v153 - *(_QWORD *)(v914 + 64)) >> 3) )
          {
            --v153;
            v7 = *v153;
            v637 = (*v153)->__align == 1;
            --v7->__align;
            if ( v637 )
              (*(void (__fastcall **)(sem_t *, signed __int64))(*(&v7->__align + 1) + 48))(v7, v152);
          }
          goto LABEL_572;
        case 68:
          v156 = *(_QWORD *)(v16 - 8);
          v929 = v16;
          v7 = *(sem_t **)(v16 - 8);
          LODWORD(v157) = PyObject_GetIter(v7);
          v637 = (*(_QWORD *)v156)-- == 1LL;
          v15 = v157;
          v16 = v929;
          if ( v637 )
          {
            v7 = (sem_t *)v156;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v156 + 8) + 48LL))(v156);
            v16 = v929;
          }
          if ( !v15 )
            goto LABEL_1630;
          *(_QWORD *)(v16 - 8) = v15;
          if ( *(_BYTE *)v20 != 93 )
            goto LABEL_19;
          v158 = *(_BYTE *)(v20 + 2);
          v159 = *(_BYTE *)(v20 + 1);
          v20 += 3LL;
          v21 = (v158 << 8) + v159;
          v7 = (sem_t *)v15;
LABEL_80:
          v924 = v16;
          LODWORD(v70) = (*(int (**)(void))(*(&v7->__align + 1) + 224))();
          v15 = v70;
          v16 = v924;
          if ( !v70 )
          {
            if ( *((_QWORD *)PyThreadState_Current + 9) )
            {
              v7 = (sem_t *)PyExc_StopIteration;
              v572 = PyErr_ExceptionMatches(PyExc_StopIteration, a2);
              v16 = v924;
              if ( !v572 )
                goto LABEL_364;
              PyErr_Clear(v7, a2);
              v16 = v924;
            }
            v15 = *(__int64 **)(v924 - 8);
            v16 -= 8LL;
            v637 = (*v15)-- == 1;
            if ( v637 )
            {
              v219 = v16;
              v7 = (sem_t *)v15;
              (*(void (__fastcall **)(__int64 *))(v15[1] + 48))(v15);
              v16 = v219;
            }
            v20 += v21;
            goto LABEL_19;
          }
          *(_QWORD *)v924 = v70;
          a2 = (const char *)(v924 + 8);
          if ( *(_BYTE *)v20 == 125 )
          {
            next_instr = v20 + 3;
            v4 = v919 + 8LL * (*(_BYTE *)(v20 + 1) + (*(_BYTE *)(v20 + 2) << 8));
            v7 = *(sem_t **)v4;
            *(_QWORD *)v4 = v70;
            if ( v7 )
            {
LABEL_56:
              v637 = v7->__align-- == 1;
              if ( v637 )
              {
LABEL_57:
                v54 = v16;
                (*(void (**)(void))(*(&v7->__align + 1) + 48))();
                v16 = v54;
              }
            }
            continue;
          }
          if ( *(_BYTE *)v20 != 92 )
          {
            v16 = v924 + 8;
            goto LABEL_19;
          }
          v160 = *(_BYTE *)(v20 + 2);
          v161 = *(_BYTE *)(v20 + 1);
          v7 = (sem_t *)v70;
          v20 += 3LL;
          v16 = v924 + 8;
          v21 = (v160 << 8) + v161;
LABEL_162:
          v104 = (void *)*(&v7->__align + 1);
          v16 -= 8LL;
          if ( v104 == &PyTuple_Type )
          {
            v9 = v21;
            if ( *(&v7->__align + 2) == v21 )
            {
              if ( v21 )
              {
                v105 = (unsigned int)(v21 - 1);
                v4 = *(&v7->__align + v21 - 1 + 3);
                ++*(_QWORD *)v4;
                *(_QWORD *)v16 = v4;
                if ( v21 != 1 )
                {
                  v9 = *(&v7->__align + v21 - 2 + 3);
                  ++*(_QWORD *)v9;
                  *(_QWORD *)(v16 + 8) = v9;
                  if ( v21 != 2 )
                  {
                    v106 = (_QWORD *)*(&v7->__align + v21 - 3 + 3);
                    ++*v106;
                    *(_QWORD *)(v16 + 16) = v106;
                    if ( v21 != 3 )
                    {
                      v10 = *(&v7->__align + v21 - 4 + 3);
                      v107 = v21 - 5;
                      ++*(_QWORD *)v10;
                      *(_QWORD *)(v16 + 24) = v10;
                      if ( v21 != 4 )
                      {
                        v108 = v16 + 40;
                        v109 = v21 - 6;
                        v110 = (_QWORD *)*(&v7->__align + v107 + 3);
                        ++*v110;
                        for ( *(_QWORD *)(v16 + 32) = v110; v109 != -1; *(_QWORD *)(v108 - 8) = v112 )
                        {
                          v111 = v109--;
                          v108 += 8LL;
                          v112 = (_QWORD *)*(&v7->__align + v111 + 3);
                          ++*v112;
                        }
                      }
                    }
                  }
                }
                v16 += 8 * v105 + 8;
              }
              v637 = v7->__align-- == 1;
              if ( v637 )
              {
                v113 = v16;
                (*(void (**)(void))(*(&v7->__align + 1) + 48))();
                v16 = v113;
              }
              goto LABEL_19;
            }
            v299 = v7;
          }
          else
          {
            v299 = v7;
            v9 = v21;
            if ( v104 == &PyList_Type && *(&v7->__align + 2) == v21 )
            {
              v9 = *(&v7->__align + 3);
              a2 = (const char *)(unsigned int)(v21 - 1);
              v302 = 1;
              if ( v21 )
              {
                v4 = (signed int)a2;
                v7 = *(sem_t **)(v9 + 8LL * (signed int)a2);
                ++v7->__align;
                *(_QWORD *)v16 = v7;
                if ( v21 != 1 )
                {
                  v10 = *(_QWORD *)(v9 + 8LL * (v21 - 2));
                  ++*(_QWORD *)v10;
                  *(_QWORD *)(v16 + 8) = v10;
                  if ( v21 != 2 )
                  {
                    v4 = (unsigned int)(v21 - 4);
                    v683 = *(_QWORD **)(v9 + 8LL * (v21 - 3));
                    ++*v683;
                    *(_QWORD *)(v16 + 16) = v683;
                    if ( (_DWORD)v4 != -1 )
                    {
                      v7 = (sem_t *)(signed int)v4;
                      v684 = v21 - 5;
                      v685 = *(_QWORD **)(v9 + 8LL * (signed int)v4);
                      ++*v685;
                      *(_QWORD *)(v16 + 24) = v685;
                      if ( v21 != 4 )
                      {
                        v10 = v684;
                        v686 = v16 + 40;
                        v687 = v21 - 6;
                        v688 = *(_QWORD **)(v9 + 8LL * v684);
                        ++*v688;
                        for ( *(_QWORD *)(v16 + 32) = v688; v687 != -1; *(_QWORD *)(v686 - 8) = v4 )
                        {
                          v689 = v687--;
                          v686 += 8LL;
                          v4 = *(_QWORD *)(v9 + 8 * v689);
                          ++*(_QWORD *)v4;
                        }
                      }
                    }
                  }
                }
                v16 += 8LL * (_QWORD)a2 + 8;
                v302 = 1;
              }
              goto LABEL_492;
            }
          }
          v994 = (FILE *)v16;
          v1027 = v16 + 8 * v9;
          v757 = v16 + 8 * v9;
          v7 = v299;
          LODWORD(v758) = PyObject_GetIter(v299);
          v759 = v758;
          v16 = (__int64)v994;
          if ( !v758 )
          {
            j = (_QWORD *)v1027;
            v760 = 0;
LABEL_1413:
            v1028 = v299;
            v771 = j;
            for ( i = 0LL; v760 - (signed int)i > 0; ++i )
            {
              a2 = (const char *)v771[i];
              v637 = (*(_QWORD *)a2)-- == 1LL;
              if ( v637 )
              {
                v7 = (sem_t *)v771[i];
                v774 = v16;
                (*(void (**)(void))(*(&v7->__align + 1) + 48))();
                v16 = v774;
              }
            }
            v299 = v1028;
            if ( v759 )
            {
              v637 = (*(_QWORD *)v759)-- == 1LL;
              if ( v637 )
              {
                v773 = v16;
                v7 = (sem_t *)v759;
                (*(void (__fastcall **)(__int64))(*(_QWORD *)(v759 + 8) + 48LL))(v759);
                v16 = v773;
              }
            }
            v302 = 2;
LABEL_492:
            v637 = v299->__align-- == 1;
            if ( v637 )
            {
              v521 = v16;
              v7 = v299;
              (*(void (__fastcall **)(sem_t *, const char *))(*(&v299->__align + 1) + 48))(v299, a2);
              v16 = v521;
            }
LABEL_494:
            if ( v302 != 1 )
            {
              v14 = (sem_t *)v16;
              v303 = v20;
              v10 = (unsigned int)v302;
              goto LABEL_496;
            }
            goto LABEL_364;
          }
          if ( v21 <= 0 )
          {
            v760 = 0;
            j = (_QWORD *)v1027;
          }
          else
          {
            streamg = v994;
            j = (_QWORD *)v757;
            v760 = 0;
            LODWORD(v761) = (*(int (__fastcall **)(__int64))(*(_QWORD *)(v758 + 8) + 224LL))(v758);
            v16 = (__int64)streamg;
            if ( !v761 )
              goto LABEL_1423;
            a2 = (const char *)1;
            v760 = 1;
            *(_QWORD *)(v1027 - 8) = v761;
            j = (_QWORD *)(v1027 - 8);
            if ( 1 == v21 )
              goto LABEL_1405;
            LODWORD(v762) = (*(int (__fastcall **)(__int64))(*(_QWORD *)(v759 + 8) + 224LL))(v759);
            v16 = (__int64)streamg;
            if ( !v762 )
              goto LABEL_1423;
            LOBYTE(v760) = 2;
            *(_QWORD *)(v1027 - 16) = v762;
            j = (_QWORD *)(v1027 - 16);
            if ( v760 == v21 )
              goto LABEL_1405;
            LODWORD(v763) = (*(int (__fastcall **)(__int64))(*(_QWORD *)(v759 + 8) + 224LL))(v759);
            v16 = (__int64)streamg;
            if ( !v763 )
              goto LABEL_1423;
            LOBYTE(v760) = 3;
            *(_QWORD *)(v1027 - 24) = v763;
            j = (_QWORD *)(v1027 - 24);
            if ( v760 == v21 )
              goto LABEL_1405;
            LODWORD(v764) = (*(int (__fastcall **)(__int64))(*(_QWORD *)(v759 + 8) + 224LL))(v759);
            v16 = (__int64)streamg;
            if ( !v764 )
              goto LABEL_1423;
            LOBYTE(v760) = 4;
            *(_QWORD *)(v1027 - 32) = v764;
            j = (_QWORD *)(v1027 - 32);
            if ( v760 == v21 )
              goto LABEL_1405;
            LODWORD(v765) = (*(int (__fastcall **)(__int64))(*(_QWORD *)(v759 + 8) + 224LL))(v759);
            v16 = (__int64)streamg;
            if ( !v765 )
              goto LABEL_1423;
            a2 = (const char *)(v1027 - 40);
            *(_QWORD *)(v1027 - 40) = v765;
            v760 = 5;
            j = (_QWORD *)(v1027 - 40);
            if ( 5 == v21 )
              goto LABEL_1405;
            LODWORD(v766) = (*(int (__fastcall **)(__int64))(*(_QWORD *)(v759 + 8) + 224LL))(v759);
            v16 = (__int64)streamg;
            if ( !v766 )
            {
LABEL_1423:
              if ( !*((_QWORD *)PyThreadState_Current + 9) )
                goto LABEL_1731;
              v7 = (sem_t *)PyExc_StopIteration;
              v1030 = v16;
              v783 = PyErr_ExceptionMatches(PyExc_StopIteration, a2);
              v16 = v1030;
              if ( v783 )
              {
                PyErr_Clear(v7, a2);
                v16 = v1030;
              }
              v9 = (signed __int64)PyThreadState_Current;
              if ( !*((_QWORD *)PyThreadState_Current + 9) )
              {
LABEL_1731:
                v775 = &s1;
                if ( v760 != 1 )
                  v775 = "-> %2d  %s" + 9;
                v7 = (sem_t *)PyExc_ValueError;
                a2 = "need more than %d value%s to unpack";
                v776 = v16;
                PyErr_Format(PyExc_ValueError, "need more than %d value%s to unpack", (unsigned int)v760, v775);
                v16 = v776;
              }
              goto LABEL_1413;
            }
            v760 = 6;
            *(_QWORD *)(v1027 - 48) = v766;
            for ( j = (_QWORD *)(v1027 - 48); v760 != v21; *j = v768 )
            {
              v767 = (FILE *)v16;
              LODWORD(v768) = (*(int (__fastcall **)(_QWORD))(*(_QWORD *)(v759 + 8) + 224LL))(v759);
              v16 = (__int64)v767;
              if ( !v768 )
                goto LABEL_1423;
              --j;
              ++v760;
            }
          }
LABEL_1405:
          v7 = (sem_t *)v759;
          streamh = (FILE *)v16;
          LODWORD(v769) = PyIter_Next(v759);
          v16 = (__int64)streamh;
          if ( v769 )
          {
            v637 = (*(_QWORD *)v769)-- == 1LL;
            if ( v637 )
            {
              (*(void (__fastcall **)(__int64))(*(_QWORD *)(v769 + 8) + 48LL))(v769);
              v16 = (__int64)streamh;
            }
            v7 = (sem_t *)PyExc_ValueError;
            a2 = "too many values to unpack";
            v770 = v16;
            PyErr_SetString(PyExc_ValueError, "too many values to unpack");
            v16 = v770;
          }
          else
          {
            a2 = (const char *)PyThreadState_Current;
            if ( !*((_QWORD *)PyThreadState_Current + 9) )
            {
              v637 = (*(_QWORD *)v759)-- == 1LL;
              if ( v637 )
              {
                v7 = (sem_t *)v759;
                (*(void (__fastcall **)(__int64))(*(_QWORD *)(v759 + 8) + 48LL))(v759);
              }
              v16 = v1027;
LABEL_491:
              v302 = 1;
              goto LABEL_492;
            }
          }
          goto LABEL_1413;
        case 24:
          v93 = v16;
          v162 = v16 - 8;
          v94 = *(_QWORD *)(v93 - 16);
          v95 = *(_QWORD *)(v93 - 8);
          if ( *(_UNKNOWN **)(v94 + 8) == &PyInt_Type
            && *(_UNKNOWN **)(v95 + 8) == &PyInt_Type
            && ((v163 = *(_QWORD *)(v95 + 16),
                 v7 = (sem_t *)(*(_QWORD *)(v94 + 16) - v163),
                 (((unsigned __int64)v7 ^ *(_QWORD *)(v94 + 16)) & 0x8000000000000000LL) == 0LL)
             || (((unsigned __int64)v7 ^ v163) & 0x8000000000000000LL) != 0LL) )
          {
            v164 = v162;
            LODWORD(v165) = PyInt_FromLong(v7);
            v16 = v164;
            v15 = v165;
          }
          else
          {
            v7 = *(sem_t **)(v93 - 16);
            v966 = v162;
            LODWORD(v393) = sub_561E20(v7, *(_QWORD *)(v93 - 8), 8LL);
            v15 = v393;
            v16 = v966;
            if ( v393 == &Py_NotImplementedStruct )
            {
              if ( !--Py_NotImplementedStruct )
              {
                (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
                v16 = v966;
              }
              v394 = v16;
              v7 = (sem_t *)PyExc_TypeError;
              v15 = 0LL;
              v395 = *(_QWORD *)(*(_QWORD *)(v95 + 8) + 24LL);
              PyErr_Format(
                PyExc_TypeError,
                "unsupported operand type(s) for %.100s: '%.100s' and '%.100s'",
                "-",
                *(_QWORD *)(*(_QWORD *)(v94 + 8) + 24LL));
              v16 = v394;
            }
          }
          goto LABEL_136;
        case 86:
          PyFrameObject = v914;
          v66 = *(_QWORD *)(v16 - 8);
          *(_QWORD *)(v914 + 72) = v16 - 8;
          v63 = 64;
          goto LABEL_73;
        case 103:
          v7 = (sem_t *)v21;
          v931 = v16;
          LODWORD(v172) = PyList_New(v21);
          v15 = (__int64 *)v172;
          v16 = v931;
          if ( !v172 )
            goto LABEL_364;
          v10 = v931;
          v9 = (unsigned int)(v21 - 1);
          if ( v21 - 1 >= 0 )
          {
            v4 = (signed int)v9;
            *(_QWORD *)(*(_QWORD *)(v172 + 24) + 8LL * (signed int)v9) = *(_QWORD *)(v931 - 8);
            if ( v21 != 1 )
            {
              v173 = v21 - 3;
              *(_QWORD *)(*(_QWORD *)(v172 + 24) + 8LL * (v21 - 2)) = *(_QWORD *)(v931 - 16);
              if ( v21 != 2 )
              {
                v7 = *(sem_t **)(v931 - 24);
                v174 = v21 - 4;
                v175 = (_QWORD *)(v931 - 24);
                for ( *(_QWORD *)(*(_QWORD *)(v172 + 24) + 8LL * v173) = v7; v174 != -1; *(_QWORD *)(v4 + 8 * v176) = *v175 )
                {
                  --v175;
                  v4 = v15[3];
                  v176 = v174--;
                }
              }
            }
            v10 = v931 + 8 * ~(unsigned __int64)(unsigned int)v9;
          }
LABEL_210:
          v16 = v10 + 8;
          *(_QWORD *)v10 = v15;
          goto LABEL_19;
        case 30:
        case 31:
        case 32:
        case 33:
          v177 = 0LL;
          if ( !(opcode & 2) )
          {
            v177 = *(_QWORD *)(v16 - 8);
            v16 -= 8LL;
          }
          v178 = 0LL;
          if ( opcode & 1 )
          {
            v178 = *(_QWORD *)(v16 - 8);
            v16 -= 8LL;
          }
          v179 = *(_QWORD *)(v16 - 8);
          v180 = *(_QWORD **)(v179 + 8);
          v181 = v180[13];
          if ( v181
            && *(_QWORD *)(v181 + 32)
            && (!v178
             || (v182 = *(_QWORD *)(v178 + 8), v183 = *(_QWORD *)(v182 + 168), v183 & 0x1800000)
             || (v4 = *(_QWORD *)(v182 + 96)) != 0 && v183 & 0x20000 && *(_QWORD *)(v4 + 304))
            && (!v177
             || (v184 = *(_QWORD *)(v177 + 8), v185 = *(_QWORD *)(v184 + 168), v185 & 0x1800000)
             || (v514 = *(_QWORD *)(v184 + 96)) != 0 && v185 & 0x20000 && *(_QWORD *)(v514 + 304)) )
          {
            if ( v178 )
            {
              v186 = *(_QWORD *)(v178 + 8);
              v187 = *(_QWORD *)(v186 + 168);
              if ( v187 & 0x800000 )
              {
                v188 = *(_QWORD *)(v178 + 16);
              }
              else
              {
                v511 = *(_QWORD *)(v186 + 96);
                if ( !v511 || !(v187 & 0x20000) || !*(_QWORD *)(v511 + 304) )
                {
LABEL_1645:
                  v7 = (sem_t *)PyExc_TypeError;
                  v880 = v16;
                  PyErr_SetString(PyExc_TypeError, "slice indices must be integers or None or have an __index__ method");
                  v16 = v880;
LABEL_877:
                  v15 = 0LL;
                  goto LABEL_302;
                }
                v180 = 0LL;
                v7 = (sem_t *)v178;
                v512 = v16;
                LODWORD(v513) = PyNumber_AsSsize_t(v178, 0LL);
                v188 = v513;
                v16 = v512;
                if ( v513 == -1LL )
                {
                  v180 = PyThreadState_Current;
                  if ( *((_QWORD *)PyThreadState_Current + 9) )
                    goto LABEL_877;
                }
              }
            }
            else
            {
              v188 = 0LL;
            }
            if ( v177 )
            {
              v189 = *(_QWORD *)(v177 + 8);
              v190 = *(_QWORD *)(v189 + 168);
              if ( v190 & 0x800000 )
              {
                v191 = *(_QWORD *)(v177 + 16);
              }
              else
              {
                v554 = *(_QWORD *)(v189 + 96);
                if ( !v554 || !(v190 & 0x20000) || !*(_QWORD *)(v554 + 304) )
                  goto LABEL_1645;
                v180 = 0LL;
                v7 = (sem_t *)v177;
                v555 = v16;
                LODWORD(v556) = PyNumber_AsSsize_t(v177, 0LL);
                v191 = v556;
                v16 = v555;
                if ( v556 == -1LL )
                {
                  v9 = (signed __int64)PyThreadState_Current;
                  if ( *((_QWORD *)PyThreadState_Current + 9) )
                    goto LABEL_877;
                }
              }
            }
            else
            {
              v191 = 0x7FFFFFFFFFFFFFFFLL;
            }
            v192 = *(_QWORD *)(v179 + 8);
            v7 = *(sem_t **)(v192 + 104);
            if ( v7 )
            {
              v193 = (int (__fastcall *)(__int64, unsigned __int64, unsigned __int64, signed __int64, signed __int64, signed __int64))v7[1].__align;
              if ( v193 )
              {
                v9 = v191 >> 63;
                v4 = v188 >> 63;
                v10 = v191 >> 63;
                LOBYTE(v10) = (v188 >> 63) | (v191 >> 63);
                if ( !(_BYTE)v10 || !v7->__align )
                  goto LABEL_301;
                v506 = v16;
                LODWORD(v507) = ((int (__fastcall *)(__int64, _QWORD *, __int64, signed __int64, signed __int64, signed __int64))v7->__align)(
                                  v179,
                                  v180,
                                  v7->__align,
                                  v4,
                                  v9,
                                  v10);
                v16 = v506;
                v4 = v191 >> 63;
                if ( v507 >= 0 )
                {
                  if ( v188 >> 63 )
                    v188 += v507;
                  if ( v191 >> 63 )
                    v191 += v507;
                  v193 = (int (__fastcall *)(__int64, unsigned __int64, unsigned __int64, signed __int64, signed __int64, signed __int64))v7[1].__align;
LABEL_301:
                  v194 = v16;
                  v7 = (sem_t *)v179;
                  LODWORD(v195) = v193(v179, v188, v191, v4, v9, v10);
                  v16 = v194;
                  v15 = v195;
                  goto LABEL_302;
                }
                goto LABEL_877;
              }
            }
            v900 = *(_QWORD *)(v192 + 112);
            if ( v900 && (v1032 = *(_QWORD *)(v192 + 112), *(_QWORD *)(v900 + 8)) )
            {
              v7 = (sem_t *)v188;
              v970 = v16;
              LODWORD(v901) = PySlice_FromIndices(v188, v191);
              v516 = v901;
              v16 = v970;
              if ( !v901 )
                goto LABEL_877;
              v7 = (sem_t *)v179;
              LODWORD(v902) = (*(int (__fastcall **)(__int64, __int64))(v1032 + 8))(v179, v901);
              v517 = *(_QWORD *)v516;
              v15 = v902;
              v16 = v970;
              v637 = (*(_QWORD *)v516)-- == 1LL;
              if ( v637 )
              {
LABEL_884:
                v7 = (sem_t *)v516;
                (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v516 + 8) + 48LL))(v516, v517);
                v16 = v970;
                goto LABEL_302;
              }
            }
            else
            {
              v7 = (sem_t *)PyExc_TypeError;
              v903 = v16;
              v15 = 0LL;
              PyErr_Format(PyExc_TypeError, "'%.200s' object is unsliceable", *(_QWORD *)(v192 + 24), v4);
              v16 = v903;
            }
          }
          else
          {
            v7 = (sem_t *)v178;
            v970 = v16;
            v15 = 0LL;
            LODWORD(v515) = PySlice_New(v178, v177, 0LL);
            v516 = v515;
            v16 = v970;
            if ( v515 )
            {
              v517 = v515;
              v7 = (sem_t *)v179;
              LODWORD(v518) = PyObject_GetItem(v179, v515);
              v637 = (*(_QWORD *)v516)-- == 1LL;
              v15 = v518;
              v16 = v970;
              if ( v637 )
                goto LABEL_884;
            }
          }
LABEL_302:
          v637 = (*(_QWORD *)v179)-- == 1LL;
          if ( v637 )
          {
            v553 = v16;
            v7 = (sem_t *)v179;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v179 + 8) + 48LL))(v179);
            v16 = v553;
          }
          if ( v178 )
          {
            v637 = (*(_QWORD *)v178)-- == 1LL;
            if ( v637 )
            {
              v557 = v16;
              v7 = (sem_t *)v178;
              (*(void (__fastcall **)(__int64))(*(_QWORD *)(v178 + 8) + 48LL))(v178);
              v16 = v557;
            }
          }
          if ( v177 )
          {
            v637 = (*(_QWORD *)v177)-- == 1LL;
            if ( v637 )
            {
              v196 = v16;
              v7 = (sem_t *)v177;
              (*(void (__fastcall **)(__int64))(*(_QWORD *)(v177 + 8) + 48LL))(v177);
              v16 = v196;
            }
          }
LABEL_18:
          *(_QWORD *)(v16 - 8) = v15;
          if ( v15 )
            goto LABEL_19;
          goto LABEL_364;
        case 60:
          v146 = *(_QWORD *)(v16 - 8);
          v197 = *(_QWORD *)(v16 - 16);
          v198 = (_QWORD *)(v16 - 24);
          v199 = *v198;
          v932 = v198;
          v200 = PyObject_SetItem(v197, v146, *v198);
          v637 = (*(_QWORD *)v199)-- == 1LL;
          v1005 = v200;
          v16 = (__int64)v932;
          if ( v637 )
          {
            (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v199 + 8) + 48LL))(v199, v146);
            v16 = (__int64)v932;
          }
          v637 = (*(_QWORD *)v197)-- == 1LL;
          if ( v637 )
          {
            v201 = v16;
            (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v197 + 8) + 48LL))(v197, v146);
            v16 = v201;
          }
          goto LABEL_232;
        case 2:
          v202 = *(_QWORD *)(v16 - 8);
          *(_QWORD *)(v16 - 8) = *(_QWORD *)(v16 - 16);
          *(_QWORD *)(v16 - 16) = v202;
          next_instr = v20;
          continue;
        case 55:
          v203 = v16;
          v204 = v16 - 8;
          v205 = *(_QWORD *)(v203 - 16);
          v206 = *(_QWORD *)(v203 - 8);
          v207 = *(void **)(v205 + 8);
          if ( v207 != &PyInt_Type )
          {
            if ( v207 == &PyString_Type && *(_UNKNOWN **)(v206 + 8) == &PyString_Type )
            {
              v7 = *(sem_t **)(v203 - 16);
              v420 = v204;
              LODWORD(v421) = sub_558820(v7, *(_QWORD *)(v203 - 8), v914, v20);
              v16 = v420;
              v15 = v421;
              goto LABEL_320;
            }
LABEL_318:
            v7 = *(sem_t **)(v203 - 16);
            v208 = v204;
            LODWORD(v209) = PyNumber_InPlaceAdd(v7, *(_QWORD *)(v203 - 8));
            v16 = v208;
            v15 = v209;
            goto LABEL_319;
          }
          if ( *(_UNKNOWN **)(v206 + 8) != &PyInt_Type )
            goto LABEL_318;
          v223 = *(_QWORD *)(v205 + 16);
          v224 = *(_QWORD *)(v206 + 16);
          v7 = (sem_t *)(v224 + v223);
          if ( ((v224 + v223) ^ v223) < 0 && (((unsigned __int64)v7 ^ v224) & 0x8000000000000000LL) != 0LL )
            goto LABEL_318;
LABEL_369:
          v225 = v204;
          LODWORD(v226) = PyInt_FromLong(v7);
          v16 = v225;
          v15 = v226;
LABEL_319:
          v637 = (*(_QWORD *)v205)-- == 1LL;
          if ( v637 )
          {
LABEL_411:
            v250 = v16;
            v7 = (sem_t *)v205;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v205 + 8) + 48LL))(v205);
            v16 = v250;
          }
LABEL_320:
          v637 = (*(_QWORD *)v206)-- == 1LL;
          if ( v637 )
          {
            v422 = v16;
            v7 = (sem_t *)v206;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v206 + 8) + 48LL))(v206);
            v16 = v422;
          }
          *(_QWORD *)(v203 - 16) = v15;
          if ( !v15 )
            goto LABEL_364;
          goto LABEL_19;
        case 20:
          v93 = v16;
          v210 = *(_QWORD *)(v16 - 16);
          v211 = *(_QWORD *)(v16 - 8);
          v933 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v212) = PyNumber_Multiply(v7, *(_QWORD *)(v16 - 8));
          v637 = (*(_QWORD *)v210)-- == 1LL;
          v15 = v212;
          v16 = v933;
          if ( v637 )
          {
            v7 = (sem_t *)v210;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v210 + 8) + 48LL))(v210);
            v16 = v933;
          }
          v637 = (*(_QWORD *)v211)-- == 1LL;
          if ( v637 )
          {
            v213 = v16;
            v7 = (sem_t *)v211;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v211 + 8) + 48LL))(v211);
            v16 = v213;
          }
          goto LABEL_140;
        case 136:
          v214 = v21;
          v4 = v16;
          v15 = *(__int64 **)(v1014 + 8LL * v21);
          if ( (_UNKNOWN *)v15[1] == &PyCell_Type )
          {
            v10 = v15[2];
            if ( v10 )
            {
              ++*(_QWORD *)v10;
              v215 = v15[2];
              if ( v215 )
              {
                v16 += 8LL;
                *(_QWORD *)v4 = v215;
                goto LABEL_19;
              }
            }
          }
          else
          {
            v7 = (sem_t *)"../Objects/cellobject.c";
            v821 = v16;
            PyErr_BadInternalCall("../Objects/cellobject.c", 24LL);
            v16 = v821;
          }
          v1005 = -1;
          if ( !*((_QWORD *)PyThreadState_Current + 9) )
          {
            v998 = v16;
            v822 = *(_QWORD *)(PyObject_co + 32);
            v823 = *(_QWORD *)(v822 + 16);
            if ( v214 >= v823 )
            {
              v7 = (sem_t *)PyExc_NameError;
              sub_44DD3B(
                PyExc_NameError,
                "free variable '%.200s' referenced before assignment in enclosing scope",
                *(_QWORD *)(*(_QWORD *)(PyObject_co + 64) + 8 * (v214 - v823) + 24));
              v16 = v998;
            }
            else
            {
              v7 = (sem_t *)PyExc_UnboundLocalError;
              sub_44DD3B(
                PyExc_UnboundLocalError,
                "local variable '%.200s' referenced before assignment",
                *(_QWORD *)(v822 + 8 * v214 + 24));
              v16 = v998;
            }
          }
          goto LABEL_364;
        case 4:
          v10 = *(_QWORD *)(v16 - 8);
          next_instr = v20;
          v16 += 8LL;
          ++*(_QWORD *)v10;
          *(_QWORD *)(v16 - 8) = v10;
          continue;
        case 9:
          goto LABEL_360;
        case 105:
          v14 = (sem_t *)v16;
          v935 = v16;
          LODWORD(v222) = PyDict_New(v7);
          v15 = v222;
          if ( v222 == 0LL || v21 <= 5LL )
          {
            v16 = v935 + 8;
            v14->__align = (__int64)v222;
            if ( v222 == 0LL )
              goto LABEL_364;
            goto LABEL_19;
          }
          v7 = (sem_t *)v222;
          v637 = sub_54E4A0(v222, v21) == -1;
          v16 = v935;
          if ( v637 )
          {
            v637 = (*v15)-- == 1;
            if ( v637 )
            {
              v7 = (sem_t *)v15;
              (*(void (__fastcall **)(__int64 *))(v15[1] + 48))(v15);
              v16 = v935;
            }
            *(_QWORD *)v16 = 0LL;
            v15 = 0LL;
            v16 += 8LL;
            goto LABEL_364;
          }
          goto LABEL_506;
        case 12:
          v227 = *(_QWORD *)(v16 - 8);
          if ( (__int64 *)v227 == &Py_TrueStruct )
            goto LABEL_702;
          v1005 = 0;
          if ( (__int64 *)v227 == &Py_ZeroStruct || (__int64 *)v227 == &Py_NoneStruct )
            goto LABEL_373;
          v9 = *(_QWORD *)(v227 + 8);
          v10 = *(_QWORD *)(v9 + 96);
          if ( v10 && (v423 = *(int (__fastcall **)(__int64))(v10 + 80)) != 0LL )
          {
            v424 = v16;
            v7 = *(sem_t **)(v16 - 8);
            LODWORD(v425) = v423(v227);
            v16 = v424;
            v425 = (signed int)v425;
          }
          else
          {
            v428 = *(int (__fastcall ***)(_QWORD))(v9 + 112);
            if ( v428 && *v428 )
            {
              v429 = v16;
              v7 = *(sem_t **)(v16 - 8);
              LODWORD(v425) = (*v428)(v227);
              v16 = v429;
            }
            else
            {
              v519 = *(int (__fastcall ***)(__int64))(v9 + 104);
              if ( !v519 || !*v519 )
              {
LABEL_702:
                v1005 = 1;
                goto LABEL_373;
              }
              v520 = v16;
              v7 = *(sem_t **)(v16 - 8);
              LODWORD(v425) = (*v519)(v227);
              v16 = v520;
            }
          }
          if ( v425 > 0 )
            goto LABEL_702;
          v1005 = v425;
LABEL_373:
          v637 = (*(_QWORD *)v227)-- == 1LL;
          if ( v637 )
          {
            v552 = v16;
            v7 = (sem_t *)v227;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v227 + 8) + 48LL))(v227);
            v16 = v552;
          }
          if ( !v1005 )
          {
            ++Py_TrueStruct;
            *(_QWORD *)(v16 - 8) = &Py_TrueStruct;
            goto LABEL_19;
          }
          if ( v1005 > 0 )
          {
            ++Py_ZeroStruct;
            v1005 = 0;
            *(_QWORD *)(v16 - 8) = &Py_ZeroStruct;
            goto LABEL_19;
          }
LABEL_1630:
          v16 -= 8LL;
          goto LABEL_364;
        case 80:
          v62 = v15;
          PyFrameObject = v914;
          v7 = (sem_t *)v16;
          v63 = 16;
          goto LABEL_68;
        case 3:
          v15 = *(__int64 **)(v16 - 24);
          next_instr = v20;
          v228 = *(_QWORD *)(v16 - 16);
          *(_QWORD *)(v16 - 24) = *(_QWORD *)(v16 - 8);
          *(_QWORD *)(v16 - 16) = v15;
          *(_QWORD *)(v16 - 8) = v228;
          continue;
        case 111:
          v233 = *(_QWORD *)(v16 - 8);
          if ( (__int64 *)v233 != &Py_TrueStruct )
          {
            if ( (__int64 *)v233 == &Py_ZeroStruct )
              goto LABEL_179;
            v7 = *(sem_t **)(v16 - 8);
            v968 = v16;
            v432 = PyObject_IsTrue(v7, a2);
            v1005 = v432;
            v16 = v968;
            if ( v432 <= 0 )
            {
              if ( !v432 )
                goto LABEL_344;
              goto LABEL_364;
            }
            v16 = v968 - 8;
            v1005 = 0;
            v9 = *(_QWORD *)v233 - 1LL;
            *(_QWORD *)v233 = v9;
            if ( v9 )
              goto LABEL_19;
LABEL_430:
            v946 = v16;
            v7 = (sem_t *)v233;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v233 + 8) + 48LL))(v233);
LABEL_431:
            v16 = v946;
            goto LABEL_19;
          }
          v16 -= 8LL;
          if ( --Py_TrueStruct )
          {
LABEL_360:
            next_instr = v20;
            continue;
          }
          v234 = v16;
          v7 = (sem_t *)&Py_TrueStruct;
          next_instr = v20;
          (*((void (__fastcall **)(_QWORD))off_920738 + 6))(&Py_TrueStruct);
          v16 = v234;
          continue;
        case 64:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v205 = *(_QWORD *)(v16 - 16);
          v936 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v235) = sub_561E20(v7, *(_QWORD *)(v16 - 8), 112LL);
          v15 = v235;
          v16 = v936;
          if ( v235 != &Py_NotImplementedStruct )
            goto LABEL_319;
          if ( !--Py_NotImplementedStruct )
          {
            (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
            v16 = v936;
          }
          v236 = "&";
          v937 = v16;
          v237 = *(_QWORD *)(*(_QWORD *)(v205 + 8) + 24LL);
          v238 = *(_QWORD *)(*(_QWORD *)(v206 + 8) + 24LL);
          goto LABEL_392;
        case 19:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_Power(v7, *(_QWORD *)(v16 - 8), &Py_NoneStruct);
          goto LABEL_394;
        case 22:
          v241 = v16;
          v242 = *(_QWORD *)(v16 - 16);
          v243 = *(_QWORD *)(v16 - 8);
          v939 = v16 - 8;
          if ( *(_UNKNOWN **)(v242 + 8) == &PyString_Type )
          {
            v244 = *(const char **)(v16 - 8);
            v7 = *(sem_t **)(v16 - 16);
            LODWORD(v245) = PyString_Format(v242, v243);
            v16 = v939;
            v15 = v245;
          }
          else
          {
            v244 = *(const char **)(v16 - 8);
            v7 = *(sem_t **)(v16 - 16);
            LODWORD(v430) = sub_561E20(v242, v243, 32LL);
            v15 = v430;
            v16 = v939;
            if ( v430 == &Py_NotImplementedStruct )
            {
              if ( !--Py_NotImplementedStruct )
              {
                (*((void (__fastcall **)(_QWORD, _QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct, v243);
                v16 = v939;
              }
              v259 = "%";
              v944 = v16;
              v258 = *(_QWORD *)(*(_QWORD *)(v242 + 8) + 24LL);
              v431 = *(_QWORD *)(*(_QWORD *)(v243 + 8) + 24LL);
LABEL_423:
              v7 = (sem_t *)PyExc_TypeError;
              v244 = "unsupported operand type(s) for %.100s: '%.100s' and '%.100s'";
              v15 = 0LL;
              PyErr_Format(PyExc_TypeError, "unsupported operand type(s) for %.100s: '%.100s' and '%.100s'", v259, v258);
              v16 = v944;
            }
          }
LABEL_399:
          v637 = (*(_QWORD *)v242)-- == 1LL;
          if ( v637 )
          {
            v473 = v16;
            v7 = (sem_t *)v242;
            (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v242 + 8) + 48LL))(v242, v244);
            v16 = v473;
          }
          v637 = (*(_QWORD *)v243)-- == 1LL;
          if ( v637 )
          {
            v490 = v16;
            v7 = (sem_t *)v243;
            (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v243 + 8) + 48LL))(v243, v244);
            v16 = v490;
          }
          *(_QWORD *)(v241 - 16) = v15;
          if ( !v15 )
            goto LABEL_364;
          goto LABEL_19;
        case 54:
          v146 = *(_QWORD *)(v16 - 8);
          v246 = *(_QWORD *)(v16 - 16);
          v940 = v16 - 16;
          v247 = v146;
          v248 = PyDict_SetItem(*(_QWORD *)(v16 - 24), v146, v246);
          goto LABEL_406;
        case 21:
          v203 = v16;
          if ( Py_QnewFlag )
            goto LABEL_566;
          v206 = *(_QWORD *)(v16 - 8);
          v205 = *(_QWORD *)(v16 - 16);
          v941 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v249) = sub_561E20(v7, *(_QWORD *)(v16 - 8), 24LL);
          v15 = v249;
          v16 = v941;
          if ( v249 == &Py_NotImplementedStruct )
          {
            if ( !--Py_NotImplementedStruct )
            {
              (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
              v16 = v941;
            }
            v7 = (sem_t *)PyExc_TypeError;
            v825 = v16;
            v15 = 0LL;
            v826 = *(_QWORD *)(*(_QWORD *)(v206 + 8) + 24LL);
            PyErr_Format(
              PyExc_TypeError,
              "unsupported operand type(s) for %.100s: '%.100s' and '%.100s'",
              "/",
              *(_QWORD *)(*(_QWORD *)(v205 + 8) + 24LL));
            v16 = v825;
          }
          v637 = (*(_QWORD *)v205)-- == 1LL;
          if ( v637 )
            goto LABEL_411;
          goto LABEL_320;
        case 62:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v205 = *(_QWORD *)(v16 - 16);
          v942 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v251) = sub_561E20(v7, *(_QWORD *)(v16 - 8), 96LL);
          v15 = v251;
          v16 = v942;
          if ( v251 != &Py_NotImplementedStruct )
            goto LABEL_319;
          if ( !--Py_NotImplementedStruct )
          {
            (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
            v16 = v942;
          }
          v236 = "<<";
          v937 = v16;
          v237 = *(_QWORD *)(*(_QWORD *)(v205 + 8) + 24LL);
          v252 = *(_QWORD *)(*(_QWORD *)(v206 + 8) + 24LL);
          goto LABEL_392;
        case 75:
          v241 = v16;
          v16 -= 8LL;
          v242 = *(_QWORD *)(v241 - 16);
          v243 = *(_QWORD *)(v241 - 8);
          v253 = *(_QWORD *)(v242 + 8);
          v254 = *(_QWORD *)(v253 + 96);
          if ( !v254 )
            goto LABEL_419;
          if ( !(*(_BYTE *)(v253 + 168) & 8) )
            goto LABEL_419;
          v255 = *(int (__fastcall **)(__int64, __int64))(v254 + 232);
          if ( !v255 )
            goto LABEL_419;
          v997 = v16;
          v244 = *(const char **)(v241 - 8);
          v7 = *(sem_t **)(v241 - 16);
          LODWORD(v819) = v255(v242, v243);
          v15 = v819;
          v16 = v997;
          if ( v819 != &Py_NotImplementedStruct )
            goto LABEL_399;
          if ( !--Py_NotImplementedStruct )
          {
            (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
            v16 = v997;
          }
LABEL_419:
          v244 = (const char *)v243;
          v7 = (sem_t *)v242;
          v943 = v16;
          LODWORD(v256) = sub_561E20(v242, v243, 96LL);
          v15 = v256;
          v16 = v943;
          if ( v256 != &Py_NotImplementedStruct )
            goto LABEL_399;
          if ( !--Py_NotImplementedStruct )
          {
            (*((void (__fastcall **)(_QWORD, _QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct, v243);
            v16 = v943;
          }
          v944 = v16;
          v257 = *(_QWORD *)(*(_QWORD *)(v243 + 8) + 24LL);
          v258 = *(_QWORD *)(*(_QWORD *)(v242 + 8) + 24LL);
          v259 = "<<=";
          goto LABEL_423;
        case 112:
          v233 = *(_QWORD *)(v16 - 8);
          if ( (__int64 *)v233 == &Py_ZeroStruct )
          {
            v16 -= 8LL;
LABEL_359:
            if ( --Py_ZeroStruct )
              goto LABEL_360;
            v811 = v16;
            v7 = (sem_t *)&Py_ZeroStruct;
            next_instr = v20;
            (*((void (__fastcall **)(_QWORD))off_920588 + 6))(&Py_ZeroStruct);
            v16 = v811;
            continue;
          }
          if ( (__int64 *)v233 == &Py_TrueStruct )
            goto LABEL_179;
          v7 = *(sem_t **)(v16 - 8);
          v945 = v16;
          v260 = PyObject_IsTrue(v7, a2);
          v1005 = v260;
          v16 = v945;
          if ( v260 > 0 )
          {
LABEL_356:
            v1005 = 0;
            v20 = v918 + v21;
            goto LABEL_19;
          }
          if ( v260 )
            goto LABEL_364;
          v16 = v945 - 8;
LABEL_429:
          v637 = (*(_QWORD *)v233)-- == 1LL;
          if ( v637 )
            goto LABEL_430;
LABEL_19:
          v638 = Py_Ticker-- - 1 < 0;
          next_instr = v20;
          if ( !v638 )
            continue;
LABEL_20:
          if ( *(_BYTE *)next_instr == 122 )
            continue;
          a2 = (const char *)v920;
          Py_Ticker = Py_CheckInterval;
          v10 = (unsigned int)dword_8ED660;
          ++*((_DWORD *)a2 + 32);
          if ( !(_DWORD)v10 )
          {
LABEL_22:
            v7 = sem;
            if ( sem )
            {
              v637 = v920 == PyThreadState_Current;
              PyThreadState_Current = 0LL;
              if ( !v637 )
                Py_FatalError("ceval: tstate mix-up", v920);
              v921 = v16;
              PyThread_release_lock();
              v25 = sem;
              v26 = sem_wait(sem);
              v16 = v921;
              if ( v26 == -1 )
              {
                v816 = __errno_location();
                v16 = v921;
                v26 = *v816;
              }
              if ( v26 == 4 )
              {
                v996 = v16;
                v26 = sem_wait(v25);
                v16 = v996;
                if ( v26 == -1 )
                {
                  v814 = __errno_location();
                  v16 = v996;
                  v26 = *v814;
                }
                v815 = v16;
                if ( v26 == 4 )
                {
                  do
                  {
                    v26 = sem_wait(v25);
                    if ( v26 == -1 )
                      v26 = *__errno_location();
                  }
                  while ( v26 == 4 );
                  v16 = v815;
                }
              }
              if ( v26 )
              {
                v813 = v16;
                perror("sem_wait");
                v16 = v813;
              }
              v9 = (signed __int64)PyThreadState_Current;
              v7 = (sem_t *)v920;
              v637 = PyThreadState_Current == 0LL;
              PyThreadState_Current = v920;
              if ( !v637 )
                Py_FatalError("ceval: orphan tstate", v920);
              v27 = *((_QWORD *)v920 + 17);
              if ( v27 )
              {
                *((_QWORD *)v920 + 17) = 0LL;
                v14 = (sem_t *)v16;
                PyErr_SetNone(v27);
                v637 = (*(_QWORD *)v27)-- == 1LL;
                if ( v637 )
                {
                  (*(void (__fastcall **)(__int64))(*(_QWORD *)(v27 + 8) + 48LL))(v27);
                  v15 = (__int64 *)v27;
                  v10 = 2LL;
                }
                else
                {
                  v15 = (__int64 *)v27;
                  v10 = 2LL;
                }
                goto LABEL_911;
              }
            }
            continue;
          }
          v977 = (sem_t *)v16;
          v573 = Py_MakePendingCalls(v7, v920);
          v16 = (__int64)v977;
          if ( v573 >= 0 )
          {
            if ( dword_8ED660 )
              Py_Ticker = 0;
            goto LABEL_22;
          }
          v14 = v977;
          v10 = 2LL;
LABEL_911:
          v7 = v14;
LABEL_497:
          v4 = (signed __int64)PyThreadState_Current;
          if ( !*((_QWORD *)PyThreadState_Current + 9) )
          {
            PyFrameObject = v914;
            v817 = v1005;
            v11 = (signed __int64)v14;
            v818 = v15;
            goto LABEL_1517;
          }
          if ( (_DWORD)v10 == 2 )
          {
            PyFrameObject = v914;
            v304 = v15;
            goto LABEL_500;
          }
LABEL_806:
          if ( (_DWORD)v10 == 4 )
          {
            v62 = v15;
            PyFrameObject = v914;
            goto LABEL_501;
          }
          if ( (_DWORD)v10 == 1 )
            goto LABEL_9;
          v62 = v15;
          PyFrameObject = v914;
          v63 = v10;
          goto LABEL_68;
        case 63:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v205 = *(_QWORD *)(v16 - 16);
          v947 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v261) = sub_561E20(v7, *(_QWORD *)(v16 - 8), 104LL);
          v15 = v261;
          v16 = v947;
          if ( v261 != &Py_NotImplementedStruct )
            goto LABEL_319;
          if ( !--Py_NotImplementedStruct )
          {
            (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
            v16 = v947;
          }
          v236 = ">>";
          v937 = v16;
          v237 = *(_QWORD *)(*(_QWORD *)(v205 + 8) + 24LL);
          v262 = *(_QWORD *)(*(_QWORD *)(v206 + 8) + 24LL);
          goto LABEL_392;
        case 140:
        case 141:
        case 142:
          v263 = opcode + 125;
          v264 = (unsigned __int8)v21;
          v1015 = BYTE1(v21);
          v265 = ((_BYTE)opcode + 125) & 1;
          v266 = v263 & 2;
          v267 = 8LL
               * ((unsigned __int8)v21 + 2 * BYTE1(v21) - (((unsigned int)v265 < 1) - 1) - (((unsigned int)v266 < 1) - 1));
          v268 = v16 + -8 - v267;
          v269 = *(sem_t **)v268;
          v948 = (sem_t *)(v16 + -8 - v267);
          if ( *(_UNKNOWN **)(*(_QWORD *)v268 + 8LL) == &PyMethod_Type
            && (v504 = (_QWORD *)*(&v269->__align + 3)) != 0LL )
          {
            ++*v504;
            v269 = (sem_t *)*(&v269->__align + 2);
            ++v269->__align;
            v637 = (**(_QWORD **)v268)-- == 1LL;
            if ( v637 )
            {
              v505 = (FILE *)v16;
              (*(void (**)(void))(*(_QWORD *)(*(_QWORD *)v268 + 8LL) + 48LL))();
              v16 = (__int64)v505;
            }
            v7 = v948;
            ++v264;
            v948->__align = (__int64)v504;
          }
          else
          {
            ++v269->__align;
          }
          if ( v266 )
          {
            a2 = (const char *)(v16 - 8);
            v1059 = v16 - 8;
            v503 = *(sem_t **)(v16 - 8);
            if ( *(_BYTE *)(*(&v503->__align + 1) + 171) & 0x20 )
            {
              v270 = *(sem_t **)(v16 - 8);
            }
            else
            {
              LODWORD(v899) = PyDict_New(v7);
              v270 = v899;
              if ( !v899 )
              {
                v272 = 0LL;
                goto LABEL_1212;
              }
              a2 = (const char *)v503;
              v7 = v899;
              if ( PyDict_Update(v899, v503) )
              {
                v637 = v270->__align-- == 1;
                if ( v637 )
                  (*(void (__fastcall **)(sem_t *, sem_t *))(*(&v270->__align + 1) + 48))(v270, v503);
                v7 = (sem_t *)PyExc_AttributeError;
                v272 = 0LL;
                if ( PyErr_ExceptionMatches(PyExc_AttributeError, v503) )
                {
                  v904 = (void *)*(&v269->__align + 1);
                  v905 = "()";
                  v906 = *(_QWORD *)(*(&v503->__align + 1) + 24);
                  if ( v904 != &PyMethod_Type && v904 != &PyFunction_Type && v904 != &PyCFunction_Type )
                  {
                    v905 = " constructor";
                    if ( v904 != &PyClass_Type )
                    {
                      v905 = " instance";
                      if ( v904 != &PyInstance_Type )
                        v905 = (char *)&unk_602F90;
                    }
                  }
                  LODWORD(v907) = PyEval_GetFuncName(v269);
                  v7 = (sem_t *)PyExc_TypeError;
                  v908 = v905;
                  a2 = "%.200s%.200s argument after ** must be a mapping, not %.200s";
                  v272 = 0LL;
                  PyErr_Format(
                    PyExc_TypeError,
                    "%.200s%.200s argument after ** must be a mapping, not %.200s",
                    v907,
                    v908);
                }
                goto LABEL_1212;
              }
              v637 = v503->__align-- == 1;
              if ( v637 )
              {
                v7 = v503;
                (*(void (__fastcall **)(sem_t *, sem_t *))(*(&v503->__align + 1) + 48))(v503, v503);
              }
            }
          }
          else
          {
            v1059 = v16;
            v270 = 0LL;
          }
          if ( v265 )
          {
            v271 = (sem_t **)(v1059 - 8);
            v1059 = (__int64)v271;
            v272 = *v271;
            if ( !(*(_BYTE *)(*(&(*v271)->__align + 1) + 171) & 4) )
            {
              v7 = *v271;
              LODWORD(v671) = PySequence_Tuple(*v271);
              if ( !v671 )
              {
                v7 = (sem_t *)PyExc_TypeError;
                v503 = v270;
                if ( PyErr_ExceptionMatches(PyExc_TypeError, a2) )
                {
                  v874 = (void *)*(&v269->__align + 1);
                  v875 = "()";
                  v876 = *(_QWORD *)(*(&v272->__align + 1) + 24);
                  if ( v874 != &PyMethod_Type && v874 != &PyFunction_Type && v874 != &PyCFunction_Type )
                  {
                    v875 = " constructor";
                    if ( v874 != &PyClass_Type )
                    {
                      v875 = " instance";
                      if ( v874 != &PyInstance_Type )
                        v875 = (char *)&unk_602F90;
                    }
                  }
                  LODWORD(v877) = PyEval_GetFuncName(v269);
                  v7 = (sem_t *)PyExc_TypeError;
                  v878 = v875;
                  a2 = "%.200s%.200s argument after * must be a sequence, not %200s";
                  v503 = v270;
                  PyErr_Format(
                    PyExc_TypeError,
                    "%.200s%.200s argument after * must be a sequence, not %200s",
                    v877,
                    v878);
                }
                goto LABEL_1212;
              }
              v637 = v272->__align-- == 1;
              if ( v637 )
              {
                v672 = *(&v272->__align + 1);
                v7 = v272;
                v272 = v671;
                (*(void (__fastcall **)(sem_t *))(v672 + 48))(v7);
              }
              else
              {
                v272 = v671;
              }
            }
            v273 = *((_DWORD *)&v272->__align + 4);
          }
          else
          {
            v272 = 0LL;
            v273 = 0;
          }
          if ( v1015 )
          {
            a2 = (const char *)v1015;
            v7 = v270;
            LODWORD(v670) = sub_488BF0(v270, v1015, &v1059, v269);
            v270 = v670;
            if ( !v670 )
            {
              v503 = 0LL;
              goto LABEL_1212;
            }
          }
          v4 = (unsigned int)(v273 + v264);
          v274 = v273 + v264;
          if ( v274 < 0 )
          {
            a2 = (const char *)54;
            v7 = (sem_t *)"../Objects/tupleobject.c";
            PyErr_BadInternalCall("../Objects/tupleobject.c", 54LL);
LABEL_1616:
            v503 = v270;
LABEL_1212:
            v270 = v503;
            v15 = 0LL;
            goto LABEL_464;
          }
          if ( v273 + v264 )
          {
            if ( v274 <= 19 )
            {
              v10 = qword_932E60[v274];
              if ( v10 )
              {
                v275 = *(_QWORD *)(v10 + 24);
                --dword_932E00[v274];
                qword_932E60[v274] = v275;
                *(_QWORD *)v10 = 1LL;
                goto LABEL_449;
              }
            }
          }
          else
          {
            v10 = qword_932E60[0];
            if ( qword_932E60[0] )
            {
              ++*(_QWORD *)qword_932E60[0];
              goto LABEL_452;
            }
          }
          a2 = (const char *)(v274 & 0x1FFFFFFFFFFFFFFFLL);
          if ( v274 != (v274 & 0x1FFFFFFFFFFFFFFFLL) || (unsigned __int64)(8 * v274) > 0x7FFFFFFFFFFFFFD7LL )
          {
            LODWORD(v879) = PyErr_NoMemory(v7);
            v10 = v879;
            if ( !v879 )
              goto LABEL_1616;
LABEL_452:
            if ( v273 > 0 )
            {
              v278 = (_QWORD *)*(&v272->__align + 3);
              a2 = (const char *)v264;
              ++*v278;
              *(_QWORD *)(v10 + 8LL * v264 + 24) = v278;
              if ( v273 > 1 )
              {
                v279 = (_QWORD *)v272[1].__align;
                ++*v279;
                *(_QWORD *)(v10 + 8LL * (v264 + 1) + 24) = v279;
                if ( v273 > 2 )
                {
                  v280 = (_QWORD *)*(&v272[1].__align + 1);
                  v4 = (unsigned int)(v264 + 2);
                  ++*v280;
                  *(_QWORD *)(v10 + 8LL * (v264 + 2) + 24) = v280;
                  if ( v273 > 3 )
                  {
                    v281 = (_QWORD *)*(&v272[1].__align + 2);
                    a2 = (const char *)(unsigned int)(v264 + 3);
                    v282 = 4LL;
                    ++*v281;
                    *(_QWORD *)(v10 + 8LL * (v264 + 3) + 24) = v281;
                    if ( v273 > 4 )
                    {
                      do
                      {
                        v283 = (_QWORD *)*(&v272->__align + v282 + 3);
                        v284 = v264 + v282++;
                        v4 = v284;
                        ++*v283;
                        *(_QWORD *)(v10 + 8LL * v284 + 24) = v283;
                      }
                      while ( v273 > (signed int)v282 );
                    }
                  }
                }
              }
            }
            if ( v264 )
            {
              v478 = v1059;
              v479 = v264 - 2;
              v480 = v1059 - 16;
              v1059 -= 8LL;
              a2 = *(const char **)(v478 - 8);
              *(_QWORD *)(v10 + 8LL * (v264 - 1) + 24) = a2;
              if ( v264 != 1 )
              {
                v1059 = v480;
                v481 = v264 - 3;
                v4 = v479;
                *(_QWORD *)(v10 + 8LL * v479 + 24) = *(_QWORD *)(v478 - 16);
                if ( v264 != 2 )
                {
                  v1059 = v478 - 24;
                  a2 = *(const char **)(v478 - 24);
                  v482 = v264 - 4;
                  v483 = (__int64 *)(v478 - 32);
                  for ( *(_QWORD *)(v10 + 8LL * v481 + 24) = a2; v482 != -1; *(_QWORD *)(v10 + 8 * v485 + 24) = v484 )
                  {
                    v1059 = (__int64)v483;
                    v484 = *v483;
                    v485 = v482--;
                    --v483;
                  }
                }
              }
            }
            v285 = *(&v269->__align + 1);
            if ( (_UNKNOWN *)v285 == &PyCFunction_Type )
            {
              v286 = PyThreadState_Current;
              if ( !*((_DWORD *)PyThreadState_Current + 8)
                || (v7 = (sem_t *)*((_QWORD *)PyThreadState_Current + 5)) == 0LL )
              {
                a2 = (const char *)v10;
                v7 = v269;
                v287 = v10;
                LODWORD(v288) = PyCFunction_Call(v269, v10, v270);
                v10 = v287;
                v15 = v288;
                goto LABEL_462;
              }
              a2 = (const char *)*((_QWORD *)PyThreadState_Current + 7);
              v1031 = (const char *)v10;
              v870 = sub_5B63D0(v7, a2, *((_QWORD *)PyThreadState_Current + 2), 4LL, v269);
              v10 = (signed __int64)v1031;
              if ( !v870 )
              {
                a2 = v1031;
                LODWORD(v871) = PyCFunction_Call(v269, v1031, v270);
                v7 = (sem_t *)*((_QWORD *)v286 + 5);
                v15 = v871;
                v10 = (signed __int64)v1031;
                if ( !v7 )
                  goto LABEL_462;
                if ( !v871 )
                {
                  a2 = (const char *)*((_QWORD *)v286 + 7);
                  sub_563E80(v7, a2, *((_QWORD *)v286 + 2), 5LL, v269);
                  v10 = (signed __int64)v1031;
LABEL_462:
                  v637 = (*(_QWORD *)v10)-- == 1LL;
                  if ( v637 )
                  {
                    a2 = *(const char **)(v10 + 8);
                    v7 = (sem_t *)v10;
                    (*((void (__fastcall **)(_QWORD))a2 + 6))(v10);
                  }
LABEL_464:
                  if ( v270 )
                  {
                    v637 = v270->__align-- == 1;
                    if ( v637 )
                    {
                      v7 = v270;
                      (*(void (__fastcall **)(sem_t *, const char *))(*(&v270->__align + 1) + 48))(v270, a2);
                    }
                  }
                  if ( v272 )
                  {
                    v637 = v272->__align-- == 1;
                    if ( v637 )
                    {
                      v7 = v272;
                      (*(void (__fastcall **)(sem_t *, const char *))(*(&v272->__align + 1) + 48))(v272, a2);
                    }
                  }
                  v289 = (char *)v1059;
                  v637 = v269->__align-- == 1;
                  if ( v637 )
                  {
                    v7 = v269;
                    (*(void (__fastcall **)(sem_t *, const char *))(*(&v269->__align + 1) + 48))(v269, a2);
                  }
                  if ( v948 < (sem_t *)v289 )
                  {
                    v7 = (sem_t *)*((_QWORD *)v289 - 1);
                    v637 = v7->__align-- == 1;
                    if ( v637 )
                      (*(void (__fastcall **)(sem_t *, const char *))(*(&v7->__align + 1) + 48))(v7, a2);
                    if ( v948 < (sem_t *)(v289 - 8) )
                    {
                      v7 = (sem_t *)*((_QWORD *)v289 - 2);
                      v637 = v7->__align-- == 1;
                      if ( v637 )
                      {
                        a2 = (const char *)*(&v7->__align + 1);
                        (*((void (**)(void))a2 + 6))();
                      }
                      if ( v948 < (sem_t *)(v289 - 16) )
                      {
                        v7 = (sem_t *)*((_QWORD *)v289 - 3);
                        for ( k = (sem_t **)(v289 - 24); ; v7 = *k )
                        {
                          v637 = v7->__align-- == 1;
                          if ( v637 )
                            (*(void (__fastcall **)(sem_t *, const char *))(*(&v7->__align + 1) + 48))(v7, a2);
                          if ( v948 >= (sem_t *)k )
                            break;
                          --k;
                        }
                      }
                    }
                    v289 += ~(v289 - 1 - (char *)v948) & 0xFFFFFFFFFFFFFFF8LL;
                  }
                  v16 = (__int64)(v289 + 8);
                  *(_QWORD *)v289 = v15;
                  if ( v15 )
                    goto LABEL_19;
                  goto LABEL_364;
                }
                a2 = (const char *)*((_QWORD *)v286 + 7);
                v872 = sub_5B63D0(v7, a2, *((_QWORD *)v286 + 2), 6LL, v269);
                v10 = (signed __int64)v1031;
                if ( !v872 )
                  goto LABEL_462;
                v637 = (*v15)-- == 1;
                if ( v637 )
                {
                  v873 = v15[1];
                  v7 = (sem_t *)v15;
                  v15 = 0LL;
                  (*(void (__fastcall **)(sem_t *))(v873 + 48))(v7);
                  v10 = (signed __int64)v1031;
                  goto LABEL_462;
                }
              }
            }
            else
            {
              v474 = *(int (__fastcall **)(sem_t *, signed __int64, sem_t *))(v285 + 128);
              if ( !v474 )
              {
                v851 = *(_QWORD *)(v285 + 24);
                v7 = (sem_t *)PyExc_TypeError;
                a2 = "'%.200s' object is not callable";
                v852 = v10;
                v15 = 0LL;
                PyErr_Format(PyExc_TypeError, "'%.200s' object is not callable", v851, v4);
                v10 = v852;
                goto LABEL_462;
              }
              v475 = *((_DWORD *)PyThreadState_Current + 6) + 1;
              v639 = __OFSUB__(v475, Py_CheckRecursionLimit);
              v637 = v475 == Py_CheckRecursionLimit;
              v638 = v475 - Py_CheckRecursionLimit < 0;
              *((_DWORD *)PyThreadState_Current + 6) = v475;
              if ( (unsigned __int8)(v638 ^ v639) | v637
                || (v7 = (sem_t *)" while calling a Python object",
                    v849 = v10,
                    v850 = Py_CheckRecursiveCall(" while calling a Python object"),
                    v10 = v849,
                    !v850) )
              {
                a2 = (const char *)v10;
                v1019 = v10;
                v7 = v269;
                LODWORD(v476) = v474(v269, v10, v270);
                v15 = v476;
                v477 = PyThreadState_Current;
                v10 = v1019;
                --v477[6];
                if ( v15 )
                  goto LABEL_462;
                if ( !*((_QWORD *)v477 + 9) )
                {
                  v7 = (sem_t *)PyExc_SystemError;
                  a2 = "NULL result without error in PyObject_Call";
                  PyErr_SetString(PyExc_SystemError, "NULL result without error in PyObject_Call");
                  v10 = v1019;
                  goto LABEL_462;
                }
              }
            }
            v15 = 0LL;
            goto LABEL_462;
          }
          a2 = (const char *)(v273 + v264);
          v7 = (sem_t *)&PyTuple_Type;
          LODWORD(v669) = PyObject_GC_NewVar(&PyTuple_Type, a2);
          v10 = (signed __int64)v669;
          if ( !v669 )
            goto LABEL_1616;
          v274 = v273 + v264;
          if ( !(v273 + v264) )
          {
            ++dword_932E00[0];
            qword_932E60[0] = (__int64)v669;
            ++*v669;
            goto LABEL_450;
          }
LABEL_449:
          a2 = 0LL;
          v276 = v10;
          memset((void *)(v10 + 24), 0, 8 * v274);
          v10 = v276;
LABEL_450:
          v277 = (void *)(v10 - 32);
          if ( *(_QWORD *)(v10 - 16) != -2LL )
LABEL_1526:
            Py_FatalError("GC object already tracked", a2);
          *(_QWORD *)(v10 - 16) = -3LL;
          *(_QWORD *)(v10 - 32) = PyGC_generation0;
          v4 = (signed __int64)PyGC_generation0[1];
          *(_QWORD *)(v10 - 24) = v4;
          *(_QWORD *)v4 = v277;
          PyGC_generation0[1] = v277;
          goto LABEL_452;
        case 81:
          v290 = v16;
          v291 = v16 - 8;
          v292 = *(__int64 **)(v290 - 8);
          if ( v292 == &Py_NoneStruct )
          {
            v293 = &Py_NoneStruct;
            v294 = *(_QWORD *)(v290 - 16);
            *(_QWORD *)(v290 - 16) = &Py_NoneStruct;
            v295 = &Py_NoneStruct;
          }
          else if ( *(_BYTE *)(v292[1] + 170) & 0x80 )
          {
            v566 = v292[2];
            if ( v566 != 8 && v566 != 32 )
            {
              v294 = *(_QWORD *)(v290 - 16);
              *(_QWORD *)(v290 - 16) = v292;
              v292 = &Py_NoneStruct;
              v293 = &Py_NoneStruct;
              v295 = &Py_NoneStruct;
            }
            else
            {
              v567 = *(_QWORD *)(v290 - 16);
              *(_QWORD *)(v290 - 16) = v292;
              v292 = &Py_NoneStruct;
              v294 = *(_QWORD *)(v290 - 24);
              v293 = &Py_NoneStruct;
              v295 = &Py_NoneStruct;
              *(_QWORD *)(v290 - 24) = v567;
            }
          }
          else
          {
            v295 = *(__int64 **)(v290 - 16);
            v293 = *(__int64 **)(v290 - 24);
            v294 = *(_QWORD *)(v290 - 32);
            *(_QWORD *)(v290 - 16) = v292;
            *(_QWORD *)(v290 - 24) = v295;
            *(_QWORD *)(v290 - 32) = v293;
          }
          a2 = (const char *)v292;
          v7 = (sem_t *)v294;
          v1016 = v291;
          v949 = v292;
          v296 = PyObject_CallFunctionObjArgs(v294);
          v637 = (*(_QWORD *)v294)-- == 1LL;
          v15 = (__int64 *)v296;
          v297 = v949;
          v16 = v1016;
          if ( v637 )
          {
            v7 = (sem_t *)v294;
            (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v294 + 8) + 48LL))(v294, a2);
            v16 = v1016;
            v297 = v949;
          }
          if ( !v15 )
            goto LABEL_364;
          v1005 = 0;
          if ( v297 != &Py_NoneStruct )
          {
            v532 = v16;
            v533 = v297;
            v534 = PyObject_IsTrue(v15, a2);
            v16 = v532;
            v1005 = v534;
            v297 = v533;
          }
          v637 = (*v15)-- == 1;
          if ( v637 )
          {
            v827 = v16;
            v828 = v297;
            (*(void (__fastcall **)(__int64 *, const char *))(v15[1] + 48))(v15, a2);
            v16 = v827;
            v297 = v828;
          }
          v7 = (sem_t *)(unsigned int)v1005;
          if ( v1005 < 0 )
            goto LABEL_364;
          if ( v1005 )
          {
            ++Py_NoneStruct;
            *(_QWORD *)(v290 - 32) = &Py_NoneStruct;
            v16 = v290 - 24;
            v637 = (*v297)-- == 1;
            if ( v637 )
            {
              v7 = (sem_t *)v297;
              (*(void (__fastcall **)(__int64 *, const char *))(v297[1] + 48))(v297, a2);
              v16 = v290 - 24;
            }
            v637 = (*v295)-- == 1;
            if ( v637 )
            {
              v530 = v16;
              v7 = (sem_t *)v295;
              (*(void (__fastcall **)(__int64 *, const char *))(v295[1] + 48))(v295, a2);
              v16 = v530;
            }
            v637 = (*v293)-- == 1;
            if ( v637 )
            {
              v531 = v16;
              v7 = (sem_t *)v293;
              (*(void (__fastcall **)(__int64 *, const char *))(v293[1] + 48))(v293, a2);
              v16 = v531;
            }
          }
          if ( *(_BYTE *)v20 != 88 )
          {
            v1005 = 0;
            goto LABEL_364;
          }
          ++v20;
          v298 = v16;
          v1005 = 0;
LABEL_486:
          v299 = *(sem_t **)(v298 - 8);
          v16 -= 8LL;
          v300 = *(&v299->__align + 1);
          v301 = *(_QWORD *)(v300 + 168);
          if ( v301 & 0x800000 )
          {
            v9 = *(&v299->__align + 2);
            v302 = *(&v299->__align + 2);
            LOBYTE(a2) = (_DWORD)v9 == 8;
            if ( (_DWORD)v9 == 8 || (_DWORD)v9 == 32 )
            {
              v16 = v298 - 16;
              v1010 = *(_QWORD *)(v298 - 16);
            }
            goto LABEL_492;
          }
          if ( (_UNKNOWN *)v300 == &PyClass_Type || v301 & 0x80000000 && v299[5].__size[11] & 0x40 || v301 & 0x8000000 )
          {
            a2 = *(const char **)(v298 - 16);
            next_instr = v20;
            v14 = (sem_t *)(v298 - 24);
            PyErr_Restore(*(_QWORD *)(v298 - 8), a2, *(_QWORD *)(v298 - 24));
            v10 = 4LL;
            goto LABEL_911;
          }
          if ( (__int64 *)v299 != &Py_NoneStruct )
          {
            v7 = (sem_t *)PyExc_SystemError;
            a2 = "'finally' pops bad exception";
            v838 = v16;
            v302 = 2;
            PyErr_SetString(PyExc_SystemError, "'finally' pops bad exception");
            v16 = v838;
            goto LABEL_492;
          }
          goto LABEL_491;
        case 101:
          v14 = (sem_t *)v16;
          v7 = *(sem_t **)(v914 + 56);
          v306 = *(_QWORD *)(v1004 + 8LL * v21 + 24);
          if ( !v7 )
          {
            next_instr = v20;
            LODWORD(v836) = PyObject_Repr(*(_QWORD *)(v1004 + 8LL * v21 + 24));
            a2 = "no locals when loading %s";
            PyErr_Format(PyExc_SystemError, "no locals when loading %s", v836 + 36, v837);
            v10 = 2LL;
            goto LABEL_911;
          }
          v951 = v16;
          if ( *((_UNKNOWN **)&v7->__align + 1) == &PyDict_Type )
          {
            LODWORD(v307) = PyDict_GetItem(v7, v306);
            v15 = v307;
            v16 = v951;
            if ( v307 )
            {
LABEL_505:
              ++*v15;
LABEL_506:
              v16 += 8LL;
              v14->__align = (__int64)v15;
              goto LABEL_19;
            }
          }
          else
          {
            LODWORD(v498) = PyObject_GetItem(v7, v306);
            v15 = v498;
            v16 = v951;
            if ( v498 )
              goto LABEL_506;
            if ( *((_QWORD *)PyThreadState_Current + 9) )
            {
              v7 = (sem_t *)PyExc_KeyError;
              v499 = PyErr_ExceptionMatches(PyExc_KeyError, v306);
              v16 = v951;
              if ( !v499 )
                goto LABEL_364;
              PyErr_Clear(v7, v306);
              v16 = v951;
            }
          }
          v969 = v16;
          v7 = *(sem_t **)(v914 + 48);
          LODWORD(v500) = PyDict_GetItem(v7, v306);
          v15 = v500;
          v16 = v969;
          if ( !v500 )
          {
            v7 = *(sem_t **)(v914 + 40);
            LODWORD(v501) = PyDict_GetItem(v7, v306);
            v15 = v501;
            v16 = v969;
            if ( !v501 )
            {
              v7 = (sem_t *)PyExc_NameError;
              sub_44DD3B(PyExc_NameError, "name '%.200s' is not defined", v306);
              v16 = v969;
              goto LABEL_364;
            }
          }
          goto LABEL_505;
        case 11:
          v22 = *(_QWORD *)(v16 - 8);
          if ( !v22 )
            goto LABEL_1548;
          v308 = *(_QWORD *)(v22 + 8);
          v309 = *(_QWORD *)(v308 + 96);
          if ( v309 )
          {
            v310 = *(int (__fastcall **)(__int64))(v309 + 56);
            if ( v310 )
              goto LABEL_510;
          }
          v665 = *(_QWORD *)(v308 + 24);
          v987 = v16;
          v666 = "bad operand type for unary -: '%.200s'";
          goto LABEL_1195;
        case 132:
          v953 = v16 - 8;
          v313 = *(_QWORD *)(v16 - 8);
          v7 = *(sem_t **)(v16 - 8);
          LODWORD(v314) = PyFunction_New(v7, *(_QWORD *)(v914 + 48));
          v637 = (*(_QWORD *)v313)-- == 1LL;
          v15 = v314;
          v315 = (_QWORD *)v953;
          if ( v637 )
          {
            v7 = (sem_t *)v313;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v313 + 8) + 48LL))(v313);
            v315 = (_QWORD *)v953;
          }
          if ( v15 == 0LL || v21 <= 0 )
            goto LABEL_517;
          v7 = (sem_t *)v21;
          v974 = v315;
          LODWORD(v558) = PyTuple_New(v21);
          v559 = v558;
          v16 = (__int64)v974;
          if ( v558 )
          {
            v560 = (unsigned int)(v21 - 1);
            v9 = v21 - 1;
            *(_QWORD *)(v558 + 8 * v9 + 24) = *(v974 - 1);
            if ( v21 != 1 )
            {
              *(_QWORD *)(v558 + 8LL * (v21 - 2) + 24) = *(v974 - 2);
              if ( v21 != 2 )
              {
                v7 = (sem_t *)*(v974 - 3);
                v561 = v21 - 4;
                *(_QWORD *)(v558 + 8LL * (v21 - 3) + 24) = v7;
                if ( v21 != 3 )
                {
                  v562 = v21 - 5;
                  v4 = v561;
                  v10 = (signed __int64)(v974 - 4);
                  for ( *(_QWORD *)(v558 + 8LL * v561 + 24) = *(v974 - 4);
                        v562 != -1;
                        *(_QWORD *)(v558 + 8 * v9 + 24) = *(_QWORD *)v10 )
                  {
                    v10 -= 8LL;
                    v9 = v562--;
                  }
                }
              }
            }
            v315 = &v974[~(unsigned __int64)(unsigned int)v560];
            if ( (_UNKNOWN *)v15[1] == &PyFunction_Type )
            {
              if ( (__int64 *)v558 == &Py_NoneStruct )
              {
                v563 = 0LL;
                goto LABEL_977;
              }
              if ( *(_BYTE *)(*(_QWORD *)(v558 + 8) + 171LL) & 4 )
              {
                ++*(_QWORD *)v558;
                v563 = v558;
LABEL_977:
                v564 = (sem_t **)v15[4];
                if ( v564 )
                {
                  v7 = *v564;
                  v10 = (signed __int64)&(*v564)[-1].__align + 31;
                  v637 = *v564 == (sem_t *)1;
                  *v564 = (sem_t *)v10;
                  if ( v637 )
                  {
                    v7 = (sem_t *)v15[4];
                    v858 = (unsigned __int64)&v974[~(unsigned __int64)(unsigned int)v560];
                    (*(void (**)(void))(*(&v7->__align + 1) + 48))();
                    v315 = (_QWORD *)v858;
                  }
                }
                v15[4] = v563;
                v1005 = 0;
              }
              else
              {
                v7 = (sem_t *)PyExc_SystemError;
                v859 = (unsigned __int64)&v974[~(unsigned __int64)(unsigned int)v560];
                PyErr_SetString(PyExc_SystemError, &unk_608598);
                v1005 = -1;
                v315 = (_QWORD *)v859;
              }
            }
            else
            {
              v7 = (sem_t *)"../Objects/funcobject.c";
              v860 = (unsigned __int64)&v974[~(unsigned __int64)(unsigned int)v560];
              PyErr_BadInternalCall("../Objects/funcobject.c", 107LL);
              v1005 = -1;
              v315 = (_QWORD *)v860;
            }
            v637 = (*(_QWORD *)v559)-- == 1LL;
            if ( v637 )
            {
              v565 = v315;
              v7 = (sem_t *)v559;
              (*(void (__fastcall **)(__int64, _QWORD, __int64, signed __int64, signed __int64, signed __int64))(*(_QWORD *)(v559 + 8) + 48LL))(
                v559,
                *(_QWORD *)(v559 + 8),
                v560,
                v4,
                v9,
                v10);
              v315 = v565;
            }
LABEL_517:
            *v315 = v15;
            v16 = (__int64)(v315 + 1);
            goto LABEL_364;
          }
          v637 = (*v15)-- == 1;
          if ( !v637 )
            goto LABEL_1543;
          v912 = v15[1];
          v7 = (sem_t *)v15;
          v15 = 0LL;
          (*(void (__fastcall **)(sem_t *))(v912 + 48))(v7);
          v16 = (__int64)v974;
          goto LABEL_364;
        case 143:
          v318 = *(_QWORD *)(v16 - 8);
          v319 = (_QWORD *)v16;
          if ( *(_UNKNOWN **)(v318 + 8) == &PyInstance_Type )
          {
            v976 = v16;
            if ( qword_93C2E0 )
            {
              v7 = *(sem_t **)(v16 - 8);
              LODWORD(v569) = PyObject_GetAttr(v318, qword_93C2E0);
              v16 = v976;
              v15 = v569;
            }
            else
            {
              v7 = *(sem_t **)(v16 - 8);
              LODWORD(v862) = PyObject_GetAttrString(v318, "__exit__");
              v16 = v976;
              v15 = v862;
            }
            if ( !v15 )
              goto LABEL_364;
            v320 = v15;
          }
          else
          {
            v7 = *(sem_t **)(v16 - 8);
            v954 = v16;
            LODWORD(v320) = PyObject_LookupSpecial_0(v7, "__exit__", &qword_93C2E0);
            v16 = v954;
            if ( !v320 )
            {
              if ( !*((_QWORD *)PyThreadState_Current + 9) )
              {
                v7 = (sem_t *)PyExc_AttributeError;
                PyErr_SetObject(PyExc_AttributeError, qword_93C2E0);
                v16 = v954;
              }
              goto LABEL_1543;
            }
          }
          *(_QWORD *)(v16 - 8) = v320;
          if ( *(_UNKNOWN **)(v318 + 8) == &PyInstance_Type )
          {
            v321 = (const char *)qword_93C2D0;
            v975 = v16;
            if ( qword_93C2D0 )
            {
              v7 = (sem_t *)v318;
              LODWORD(v568) = PyObject_GetAttr(v318, qword_93C2D0);
              v16 = v975;
              v323 = v568;
            }
            else
            {
              v321 = "__enter__";
              v7 = (sem_t *)v318;
              LODWORD(v863) = PyObject_GetAttrString(v318, "__enter__");
              v16 = v975;
              v323 = v863;
            }
          }
          else
          {
            v321 = "__enter__";
            v7 = (sem_t *)v318;
            v955 = v16;
            LODWORD(v322) = PyObject_LookupSpecial_0(v318, "__enter__", &qword_93C2D0);
            v323 = v322;
            v16 = v955;
            if ( !v322 )
            {
              v9 = (signed __int64)PyThreadState_Current;
              if ( !*((_QWORD *)PyThreadState_Current + 9) )
              {
                v321 = (const char *)qword_93C2D0;
                v7 = (sem_t *)PyExc_AttributeError;
                PyErr_SetObject(PyExc_AttributeError, qword_93C2D0);
                v16 = v955;
              }
            }
          }
          v637 = (*(_QWORD *)v318)-- == 1LL;
          if ( v637 )
          {
            v853 = v16;
            v7 = (sem_t *)v318;
            (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v318 + 8) + 48LL))(v318, v321);
            v16 = v853;
          }
          if ( !v323 )
            goto LABEL_1543;
          a2 = 0LL;
          v7 = (sem_t *)v323;
          v956 = v16;
          v324 = PyObject_CallFunctionObjArgs(v323);
          v637 = (*(_QWORD *)v323)-- == 1LL;
          v15 = (__int64 *)v324;
          v16 = v956;
          if ( v637 )
          {
            v7 = (sem_t *)v323;
            (*(void (__fastcall **)(__int64, _QWORD))(*(_QWORD *)(v323 + 8) + 48LL))(v323, 0LL);
            v16 = v956;
          }
          if ( v15 )
          {
            v7 = (sem_t *)*(_DWORD *)(v914 + 128);
            v325 = v20 - v918 + v21;
            v326 = (v16 - *(_QWORD *)(v914 + 64)) >> 3;
            if ( (signed int)v7 > 19 )
LABEL_1540:
              Py_FatalError("XXX block stack overflow", a2);
            v4 = (unsigned int)((_DWORD)v7 + 1);
            v16 += 8LL;
            v327 = v914 + 12LL * (_QWORD)v7 + 128;
            *(_DWORD *)(v914 + 128) = v4;
            *(_DWORD *)(v327 + 4) = 143;
            *(_DWORD *)(v327 + 12) = v326;
            *(_DWORD *)(v327 + 8) = v325;
            *v319 = v15;
            goto LABEL_19;
          }
          goto LABEL_364;
        case 90:
          v328 = v16;
          v329 = v16 - 8;
          v146 = *(_QWORD *)(v328 - 8);
          v15 = *(__int64 **)(v914 + 56);
          v330 = *(_QWORD *)(v1004 + 8LL * v21 + 24);
          if ( !v15 )
          {
            v896 = v329;
            LODWORD(v897) = PyObject_Repr(*(_QWORD *)(v1004 + 8LL * v21 + 24));
            v7 = (sem_t *)PyExc_SystemError;
            PyErr_Format(PyExc_SystemError, "no locals found when storing %s", v897 + 36, v898);
            v16 = v896;
            goto LABEL_364;
          }
          v957 = v329;
          if ( (_UNKNOWN *)v15[1] == &PyDict_Type )
          {
            v331 = PyDict_SetItem(v15, v330, v146);
            v16 = v957;
            v1005 = v331;
          }
          else
          {
            v895 = PyObject_SetItem(v15, v330, v146);
            v16 = v957;
            v1005 = v895;
          }
LABEL_232:
          v637 = (*(_QWORD *)v146)-- == 1LL;
          if ( v637 )
          {
            v973 = v16;
LABEL_958:
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v146 + 8) + 48LL))(v146);
            v16 = v973;
          }
          goto LABEL_233;
        case 108:
          v958 = v16;
          v332 = *(_QWORD *)(v1004 + 8LL * v21 + 24);
          LODWORD(v333) = PyDict_GetItemString(*(_QWORD *)(v914 + 40), "__import__");
          v334 = v333;
          v335 = v958;
          if ( !v333 )
          {
            v7 = (sem_t *)PyExc_ImportError;
            v15 = 0LL;
            PyErr_SetString(PyExc_ImportError, "__import__ not found");
            v16 = v958;
            goto LABEL_364;
          }
          ++v333->__align;
          v1017 = v958;
          v336 = *(_QWORD *)(v958 - 8);
          v946 = v958 - 8;
          v337 = *(_QWORD *)(v335 - 16);
          LODWORD(v338) = PyInt_AsLong(*(_QWORD *)(v335 - 16), "__import__");
          if ( v338 != -1 || *((_QWORD *)PyThreadState_Current + 9) )
          {
            v7 = (sem_t *)5;
            v570 = *(_QWORD *)(v914 + 48);
            v339 = v332;
            *(_QWORD *)(v914 + 56);
            v571 = PyTuple_Pack(5);
            v342 = v1017;
            v343 = v571;
          }
          else
          {
            v339 = v332;
            v7 = (sem_t *)4;
            v340 = *(_QWORD *)(v914 + 48);
            *(_QWORD *)(v914 + 56);
            v341 = PyTuple_Pack(4);
            v342 = v1017;
            v343 = v341;
          }
          v637 = (*(_QWORD *)v336)-- == 1LL;
          if ( v637 )
          {
            v861 = v342;
            v7 = (sem_t *)v336;
            (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v336 + 8) + 48LL))(v336, v339);
            v342 = v861;
          }
          v637 = (*(_QWORD *)v337)-- == 1LL;
          if ( v637 )
          {
            v856 = v342;
            v7 = (sem_t *)v337;
            (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v337 + 8) + 48LL))(v337, v339);
            v342 = v856;
          }
          if ( !v343 )
          {
            v16 = v342 - 16;
            v15 = 0LL;
            v637 = v334->__align-- == 1;
            if ( v637 )
            {
              v855 = v16;
              v7 = v334;
              v15 = 0LL;
              (*(void (__fastcall **)(sem_t *, __int64))(*(&v334->__align + 1) + 48))(v334, v339);
              v16 = v855;
            }
            goto LABEL_364;
          }
          v7 = v334;
          v1018 = v342;
          LODWORD(v344) = PyEval_CallObjectWithKeywords(v334, v343, 0LL);
          v637 = v334->__align-- == 1;
          v15 = v344;
          v9 = v1018;
          if ( v637 )
          {
            v7 = v334;
            (*(void (__fastcall **)(sem_t *, __int64, __int64, signed __int64, signed __int64))(*(&v334->__align + 1)
                                                                                              + 48))(
              v334,
              v343,
              v345,
              v4,
              v1018);
            v9 = v1018;
          }
          v637 = (*(_QWORD *)v343)-- == 1LL;
          if ( v637 )
          {
            v346 = v9;
            v7 = (sem_t *)v343;
            (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v343 + 8) + 48LL))(v343, v343);
            v9 = v346;
          }
          *(_QWORD *)(v9 - 16) = v15;
          if ( !v15 )
          {
LABEL_551:
            v16 = v946;
            goto LABEL_364;
          }
          goto LABEL_431;
        case 133:
          if ( v21 == 3 )
          {
            v347 = *(_QWORD *)(v16 - 8);
            v16 -= 8LL;
          }
          else
          {
            v347 = 0LL;
          }
          v348 = *(_QWORD *)(v16 - 16);
          v349 = *(_QWORD *)(v16 - 8);
          v350 = v16 - 8;
          v959 = (sem_t *)v16;
          LODWORD(v351) = PySlice_New(*(_QWORD *)(v16 - 16), *(_QWORD *)(v16 - 8), v347);
          v637 = (*(_QWORD *)v348)-- == 1LL;
          v15 = v351;
          v7 = v959;
          if ( v637 )
          {
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v348 + 8) + 48LL))(v348);
            v7 = v959;
          }
          v637 = (*(_QWORD *)v349)-- == 1LL;
          if ( v637 )
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v349 + 8) + 48LL))(v349);
          if ( v347 )
          {
            v637 = (*(_QWORD *)v347)-- == 1LL;
            if ( v637 )
              (*(void (__fastcall **)(__int64))(*(_QWORD *)(v347 + 8) + 48LL))(v347);
          }
          goto LABEL_561;
        case 61:
          v146 = *(_QWORD *)(v16 - 8);
          v246 = *(_QWORD *)(v16 - 16);
          v940 = v16 - 16;
          v247 = *(_QWORD *)(v16 - 8);
          v248 = PyObject_DelItem(*(_QWORD *)(v16 - 16), v247);
LABEL_406:
          v1005 = v248;
          v637 = (*(_QWORD *)v246)-- == 1LL;
          v16 = v940;
          if ( v637 )
          {
            (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v246 + 8) + 48LL))(v246, v247);
            v16 = v940;
          }
          goto LABEL_232;
        case 109:
          v7 = *(sem_t **)(v16 - 8);
          v352 = (_QWORD *)v16;
          v960 = (_QWORD *)v16;
          v353 = *(_QWORD *)(v1004 + 8LL * v21 + 24);
          LODWORD(v354) = PyObject_GetAttr(v7, v353);
          v15 = v354;
          if ( v354 )
          {
            v16 = (__int64)(v960 + 1);
            *v352 = v354;
            goto LABEL_19;
          }
          v7 = (sem_t *)PyExc_AttributeError;
          v799 = PyErr_ExceptionMatches(PyExc_AttributeError, v353);
          v801 = v960;
          if ( v799 )
          {
            LODWORD(v802) = PyString_AsString(v353, v353, v800);
            v7 = (sem_t *)PyExc_ImportError;
            PyErr_Format(PyExc_ImportError, "cannot import name %.230s", v802, v803);
            v801 = v960;
          }
          *v801 = 0LL;
          v16 = (__int64)(v801 + 1);
          goto LABEL_364;
        case 27:
LABEL_566:
          v355 = *(_QWORD *)(v16 - 8);
          v356 = *(_QWORD *)(v16 - 16);
          v153 = (sem_t **)(v16 - 8);
          v961 = v16;
          v357 = *(_QWORD *)(v16 - 8);
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v358) = sub_561E20(v7, v357, 280LL);
          v15 = v358;
          v10 = v961;
          if ( v358 == &Py_NotImplementedStruct )
          {
            if ( !--Py_NotImplementedStruct )
            {
              (*((void (__fastcall **)(_QWORD, _QWORD, _QWORD, _QWORD, _QWORD, _QWORD))off_9056A8 + 6))(
                &Py_NotImplementedStruct,
                v357,
                v359,
                off_9056A8,
                v9,
                v961);
              v10 = v961;
            }
            v7 = (sem_t *)PyExc_TypeError;
            v789 = v10;
            v790 = *(_QWORD *)(*(_QWORD *)(v355 + 8) + 24LL);
            v15 = 0LL;
            PyErr_Format(
              PyExc_TypeError,
              "unsupported operand type(s) for %.100s: '%.100s' and '%.100s'",
              "/",
              *(_QWORD *)(*(_QWORD *)(v356 + 8) + 24LL));
            v10 = v789;
          }
          v637 = (*(_QWORD *)v356)-- == 1LL;
          if ( v637 )
          {
            v674 = v10;
            v7 = (sem_t *)v356;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v356 + 8) + 48LL))(v356);
            v10 = v674;
          }
          v637 = (*(_QWORD *)v355)-- == 1LL;
          if ( v637 )
          {
            v673 = v10;
            v7 = (sem_t *)v355;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v355 + 8) + 48LL))(v355);
            v10 = v673;
          }
          *(_QWORD *)(v10 - 16) = v15;
          if ( v15 )
          {
LABEL_572:
            v16 = (__int64)v153;
            goto LABEL_19;
          }
          v16 = (__int64)v153;
          goto LABEL_364;
        case 26:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v205 = *(_QWORD *)(v16 - 16);
          v962 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v360) = sub_561E20(v7, *(_QWORD *)(v16 - 8), 272LL);
          v15 = v360;
          v16 = v962;
          if ( v360 != &Py_NotImplementedStruct )
            goto LABEL_319;
          if ( !--Py_NotImplementedStruct )
          {
            (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
            v16 = v962;
          }
          v937 = v16;
          v237 = *(_QWORD *)(*(_QWORD *)(v205 + 8) + 24LL);
          v361 = *(_QWORD *)(*(_QWORD *)(v206 + 8) + 24LL);
          v236 = "//";
          goto LABEL_392;
        case 66:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v205 = *(_QWORD *)(v16 - 16);
          v963 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v362) = sub_561E20(v7, *(_QWORD *)(v16 - 8), 128LL);
          v15 = v362;
          v16 = v963;
          if ( v362 != &Py_NotImplementedStruct )
            goto LABEL_319;
          if ( !--Py_NotImplementedStruct )
          {
            (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
            v16 = v963;
          }
          v937 = v16;
          v236 = "|";
          v237 = *(_QWORD *)(*(_QWORD *)(v205 + 8) + 24LL);
          v363 = *(_QWORD *)(*(_QWORD *)(v206 + 8) + 24LL);
          goto LABEL_392;
        case 88:
          v298 = v16;
          goto LABEL_486;
        case 137:
          v233 = *(_QWORD *)(v16 - 8);
          v16 -= 8LL;
          v15 = *(__int64 **)(v1014 + 8LL * v21);
          if ( (_UNKNOWN *)v15[1] == &PyCell_Type )
          {
            v7 = (sem_t *)v15[2];
            if ( v233 )
              ++*(_QWORD *)v233;
            v15[2] = v233;
            if ( v7 )
            {
              v637 = v7->__align-- == 1;
              if ( v637 )
              {
                v364 = v16;
                (*(void (**)(void))(*(&v7->__align + 1) + 48))();
                v16 = v364;
              }
            }
          }
          else
          {
            v7 = (sem_t *)"../Objects/cellobject.c";
            v810 = v16;
            PyErr_BadInternalCall("../Objects/cellobject.c", 36LL);
            v16 = v810;
          }
          goto LABEL_429;
        case 135:
          v7 = (sem_t *)v1014;
          v16 += 8LL;
          v15 = *(__int64 **)(v1014 + 8LL * v21);
          ++*v15;
          *(_QWORD *)(v16 - 8) = v15;
          goto LABEL_19;
        case 134:
          v365 = *(_QWORD *)(v16 - 8);
          v964 = v16;
          v366 = v16;
          v367 = v21;
          v7 = (sem_t *)v365;
          a2 = *(const char **)(v914 + 48);
          LODWORD(v368) = PyFunction_New(v365, a2);
          v637 = (*(_QWORD *)v365)-- == 1LL;
          v15 = v368;
          v369 = v964;
          if ( v637 )
          {
            v7 = (sem_t *)v365;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v365 + 8) + 48LL))(v365);
            v369 = v964;
          }
          if ( !v15 )
          {
            v370 = (_QWORD *)(v369 - 8);
            v302 = 1;
LABEL_601:
            *v370 = v15;
            v16 = (__int64)(v370 + 1);
            goto LABEL_494;
          }
          v370 = (_QWORD *)(v369 - 16);
          v371 = *(_QWORD *)(v366 - 16);
          if ( (_UNKNOWN *)v15[1] != &PyFunction_Type )
          {
            a2 = (const char *)138;
            v7 = (sem_t *)"../Objects/funcobject.c";
            v857 = v370;
            PyErr_BadInternalCall("../Objects/funcobject.c", 138LL);
            v370 = v857;
LABEL_1589:
            v302 = 2;
            goto LABEL_598;
          }
          if ( (__int64 *)v371 == &Py_NoneStruct )
          {
            v373 = 0LL;
          }
          else
          {
            v372 = *(_QWORD *)(v371 + 8);
            if ( !(*(_BYTE *)(v372 + 171) & 4) )
            {
              v7 = (sem_t *)PyExc_SystemError;
              a2 = "expected tuple for closure, got '%.100s'";
              v884 = v370;
              PyErr_Format(PyExc_SystemError, "expected tuple for closure, got '%.100s'", *(_QWORD *)(v372 + 24), v4);
              v370 = v884;
              goto LABEL_1589;
            }
            ++*(_QWORD *)v371;
            v373 = v371;
          }
          v10 = v15[5];
          if ( v10 )
          {
            a2 = *(const char **)v10;
            v9 = *(_QWORD *)v10 - 1LL;
            v637 = *(_QWORD *)v10 == 1LL;
            *(_QWORD *)v10 = v9;
            if ( v637 )
            {
              v7 = (sem_t *)v15[5];
              v883 = v370;
              (*(void (**)(void))(*(&v7->__align + 1) + 48))();
              v370 = v883;
            }
          }
          v15[5] = v373;
          v302 = 1;
LABEL_598:
          v637 = (*(_QWORD *)v371)-- == 1LL;
          if ( v637 )
          {
            v882 = v370;
            v7 = (sem_t *)v371;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v371 + 8) + 48LL))(v371);
            v370 = v882;
          }
          if ( v21 <= 0 )
            goto LABEL_601;
          v7 = (sem_t *)v21;
          v983 = v370;
          LODWORD(v651) = PyTuple_New(v21);
          v652 = v651;
          v16 = (__int64)v983;
          if ( v651 )
          {
            for ( l = v983; --v21 != -1; *(_QWORD *)(v652 + 8LL * v21 + 24) = *l )
              --l;
            a2 = (const char *)v652;
            v984 = (signed __int64)&v983[-v367];
            v654 = PyFunction_SetDefaults(v15, v652);
            v7 = (sem_t *)2;
            v370 = (_QWORD *)v984;
            if ( v654 )
              v302 = 2;
            v637 = (*(_QWORD *)v652)-- == 1LL;
            if ( v637 )
            {
              v7 = (sem_t *)v652;
              (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v652 + 8) + 48LL))(v652, v652);
              v370 = (_QWORD *)v984;
            }
            goto LABEL_601;
          }
          v637 = (*v15)-- == 1;
          if ( v637 )
          {
            a2 = (const char *)v15[1];
            v7 = (sem_t *)v15;
            v15 = 0LL;
            (*((void (__fastcall **)(_QWORD))a2 + 6))(v7);
            v16 = (__int64)v983;
          }
          else
          {
            v15 = 0LL;
          }
          goto LABEL_494;
        case 76:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlaceRshift(v7, *(_QWORD *)(v16 - 8));
          goto LABEL_394;
        case 126:
          v535 = v21;
          v536 = (__int64 **)(v919 + 8LL * v21);
          v15 = *v536;
          if ( *v536 )
          {
            *v536 = 0LL;
            v637 = (*v15)-- == 1;
            if ( v637 )
            {
              v537 = v16;
              v7 = (sem_t *)v15;
              (*(void (__fastcall **)(__int64 *))(v15[1] + 48))(v15);
              v16 = v537;
            }
            goto LABEL_19;
          }
          v946 = v16;
LABEL_1545:
          LODWORD(v833) = PyTuple_GetItem(*(_QWORD *)(PyObject_co + 56), v535);
          v7 = (sem_t *)PyExc_UnboundLocalError;
          sub_44DD3B(PyExc_UnboundLocalError, "local variable '%.200s' referenced before assignment", v833);
          goto LABEL_551;
        case 10:
          v22 = *(_QWORD *)(v16 - 8);
          v952 = v16;
          v7 = *(sem_t **)(v16 - 8);
          LODWORD(v538) = PyNumber_Positive(v7);
          goto LABEL_925;
        case 104:
          v7 = 0LL;
          v971 = (__int64 **)v16;
          LODWORD(v539) = PySet_New(0LL);
          v15 = v539;
          v16 = (__int64)v971;
          if ( !v539 )
            goto LABEL_364;
          v540 = v1005;
          v541 = v971;
          while ( --v21 >= 0 )
          {
            --v541;
            v542 = *v541;
            if ( !v540 )
            {
              v7 = (sem_t *)v15;
              v540 = PySet_Add(v15, *v541);
            }
            v637 = (*v542)-- == 1;
            if ( v637 )
            {
              v7 = (sem_t *)v542;
              (*(void (__fastcall **)(__int64 *))(v542[1] + 48))(v542);
            }
          }
          v16 = (__int64)v541;
          v1005 = v540;
          if ( v540 )
            goto LABEL_955;
          v16 = (__int64)(v541 + 1);
          *v541 = v15;
          goto LABEL_19;
        case 58:
          v203 = v16;
          if ( Py_QnewFlag )
            goto LABEL_1556;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlaceDivide(v7, *(_QWORD *)(v16 - 8));
          goto LABEL_394;
        case 57:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlaceMultiply(v7, *(_QWORD *)(v16 - 8));
          goto LABEL_394;
        case 82:
          v4 = v914;
          v7 = (sem_t *)v16;
          v15 = *(__int64 **)(v914 + 56);
          if ( v15 )
          {
            ++*v15;
            v16 += 8LL;
            v7->__align = (__int64)v15;
            goto LABEL_19;
          }
          v7 = (sem_t *)PyExc_SystemError;
          v839 = v16;
          PyErr_SetString(PyExc_SystemError, "no locals");
          v16 = v839;
          goto LABEL_364;
        case 70:
          v7 = (sem_t *)"displayhook";
          v972 = v16 - 8;
          v545 = *(_QWORD *)(v16 - 8);
          LODWORD(v546) = PySys_GetObject("displayhook");
          v547 = v546;
          v16 = v972;
          if ( v546 )
          {
            if ( !v1005 )
            {
              v7 = (sem_t *)1;
              v548 = PyTuple_Pack(1);
              v15 = (__int64 *)v548;
              v1005 = -1;
              v16 = v972;
              if ( v548 )
              {
                v7 = v547;
                LODWORD(v549) = PyEval_CallObjectWithKeywords(v547, v548, 0LL);
                v16 = v972;
                if ( v549 )
                {
                  v637 = (*(_QWORD *)v549)-- == 1LL;
                  if ( v637 )
                  {
                    v7 = (sem_t *)v549;
                    (*(void (__fastcall **)(__int64))(*(_QWORD *)(v549 + 8) + 48LL))(v549);
                    v16 = v972;
                  }
                  v1005 = 0;
                }
              }
            }
          }
          else
          {
            v7 = (sem_t *)PyExc_RuntimeError;
            v15 = 0LL;
            PyErr_SetString(PyExc_RuntimeError, &unk_60C1DF);
            v1005 = -1;
            v16 = v972;
          }
          v637 = (*(_QWORD *)v545)-- == 1LL;
          if ( v637 )
          {
            v550 = v16;
            v7 = (sem_t *)v545;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v545 + 8) + 48LL))(v545);
            v16 = v550;
          }
          if ( v15 )
          {
LABEL_955:
            v637 = (*v15)-- == 1;
            if ( v637 )
            {
              v551 = v16;
              v7 = (sem_t *)v15;
              (*(void (__fastcall **)(__int64 *))(v15[1] + 48))(v15);
              v16 = v551;
            }
          }
          goto LABEL_364;
        case 99:
          if ( v21 == 2 )
          {
            v15 = *(__int64 **)(v16 - 8);
            next_instr = v20;
            v16 += 16LL;
            ++*v15;
            v4 = *(_QWORD *)(v16 - 32);
            ++*(_QWORD *)v4;
            *(_QWORD *)(v16 - 8) = v15;
            *(_QWORD *)(v16 - 16) = v4;
          }
          else
          {
            if ( v21 != 3 )
              Py_FatalError("invalid argument to DUP_TOPX (bytecode corruption?)", a2);
            v15 = *(__int64 **)(v16 - 8);
            next_instr = v20;
            v16 += 24LL;
            ++*v15;
            v894 = *(_QWORD **)(v16 - 40);
            ++*v894;
            v10 = *(_QWORD *)(v16 - 48);
            ++*(_QWORD *)v10;
            *(_QWORD *)(v16 - 8) = v15;
            *(_QWORD *)(v16 - 16) = v894;
            *(_QWORD *)(v16 - 24) = v10;
          }
          continue;
        case 73:
          v574 = *(_QWORD *)(v16 - 8);
          v575 = v16 - 16;
          v576 = *(_QWORD *)(v16 - 16);
          if ( !v574 )
            goto LABEL_1006;
          v578 = *(_QWORD *)(v16 - 8);
          if ( (__int64 *)v574 == &Py_NoneStruct )
            goto LABEL_1006;
          goto LABEL_1007;
        case 72:
          v584 = 0LL;
          goto LABEL_1034;
        case 146:
          v91 = *(_QWORD *)(v16 - 8);
          v925 = v16 - 8;
          v92 = PySet_Add(*(_QWORD *)(v16 + 8LL * -v21 - 8), *(_QWORD *)(v16 - 8));
LABEL_124:
          v1005 = v92;
          v637 = (*(_QWORD *)v91)-- == 1LL;
          v16 = v925;
          if ( !v637 )
            goto LABEL_125;
          goto LABEL_1566;
        case 50:
        case 51:
        case 52:
        case 53:
          v589 = 0LL;
          if ( !(opcode & 2) )
          {
            v589 = *(_QWORD *)(v16 - 8);
            v16 -= 8LL;
          }
          v590 = 0LL;
          if ( opcode & 1 )
          {
            v590 = *(_QWORD *)(v16 - 8);
            v16 -= 8LL;
          }
          v591 = *(_QWORD *)(v16 - 8);
          v592 = v590;
          v60 = v16 - 8;
          v593 = sub_49F870(*(_QWORD *)(v16 - 8), v590, v589, 0LL);
          v637 = (*(_QWORD *)v591)-- == 1LL;
          v1005 = v593;
          if ( v637 )
            (*(void (__fastcall **)(__int64, signed __int64))(*(_QWORD *)(v591 + 8) + 48LL))(v591, v590);
          if ( v590 )
          {
            v637 = (*(_QWORD *)v590)-- == 1LL;
            if ( v637 )
            {
              v592 = *(_QWORD *)(v590 + 8);
              (*(void (__fastcall **)(signed __int64))(v592 + 48))(v590);
            }
          }
          goto LABEL_1052;
        case 56:
          v203 = v16;
          v204 = v16 - 8;
          v205 = *(_QWORD *)(v203 - 16);
          v206 = *(_QWORD *)(v203 - 8);
          if ( *(_UNKNOWN **)(v205 + 8) == &PyInt_Type && *(_UNKNOWN **)(v206 + 8) == &PyInt_Type )
          {
            v594 = *(_QWORD *)(v206 + 16);
            v7 = (sem_t *)(*(_QWORD *)(v205 + 16) - v594);
            if ( (((unsigned __int64)v7 ^ *(_QWORD *)(v205 + 16)) & 0x8000000000000000LL) == 0LL
              || (((unsigned __int64)v7 ^ v594) & 0x8000000000000000LL) != 0LL )
            {
              goto LABEL_369;
            }
          }
          v7 = *(sem_t **)(v203 - 16);
          v595 = v204;
          LODWORD(v596) = PyNumber_InPlaceSubtract(v7, *(_QWORD *)(v203 - 8));
          v16 = v595;
          v15 = v596;
          goto LABEL_319;
        case 85:
          v597 = *(_QWORD *)(v16 - 24);
          v598 = *(__int64 **)(v16 - 8);
          v599 = v16 - 24;
          v981 = v598;
          v1021 = *(_QWORD *)(v599 + 8);
          if ( *(_BYTE *)(*(_QWORD *)(v597 + 8) + 171LL) & 4 )
          {
            if ( (__int64 *)v1021 != &Py_NoneStruct )
            {
              v602 = v597;
              v600 = v981;
              v601 = *(_QWORD *)(v599 + 8);
LABEL_1065:
              if ( v600 == &Py_NoneStruct )
                v600 = (__int64 *)v601;
              v1006 = 0;
              goto LABEL_1068;
            }
            if ( v598 != &Py_NoneStruct )
            {
              v600 = v598;
              goto LABEL_1099;
            }
            v1008 = v599;
            v885 = PyTuple_Size(v597);
            v599 = v1008;
            if ( v885 == 2 )
            {
              v600 = &Py_NoneStruct;
              LODWORD(v892) = PyTuple_GetItem(v597, 1LL);
              v888 = v1008;
              v601 = v892;
            }
            else
            {
              if ( v885 != 3 )
              {
                v600 = &Py_NoneStruct;
LABEL_1099:
                v602 = v597;
                goto LABEL_1100;
              }
              LODWORD(v886) = PyTuple_GetItem(v597, 1LL);
              v601 = v886;
              LODWORD(v887) = PyTuple_GetItem(v597, 2LL);
              v888 = v1008;
              v600 = v887;
            }
            v889 = v888;
            LODWORD(v890) = PyTuple_GetItem(v597, 0LL);
            v599 = v889;
            v602 = v890;
          }
          else
          {
            v600 = v598;
            v601 = *(_QWORD *)(v599 + 8);
            v602 = v597;
          }
          if ( (__int64 *)v601 != &Py_NoneStruct )
            goto LABEL_1065;
LABEL_1100:
          v620 = v599;
          v621 = PyThreadState_Current;
          v601 = 0LL;
          LODWORD(v622) = PyThreadState_GetFrame(PyThreadState_Current);
          v599 = v620;
          if ( v622 )
            v601 = *(_QWORD *)(v622 + 48);
          v1006 = 0;
          if ( v600 == &Py_NoneStruct )
          {
            v623 = (FILE *)v599;
            LODWORD(v624) = PyEval_GetLocals(v621);
            v599 = (signed __int64)v623;
            v600 = v624;
            v1006 = 1;
          }
          if ( v601 == 0 || v600 == 0LL )
          {
            v7 = (sem_t *)PyExc_SystemError;
            v604 = "globals and locals cannot be NULL";
            v625 = (FILE *)v599;
            PyErr_SetString(PyExc_SystemError, "globals and locals cannot be NULL");
            v1005 = -1;
            v16 = (__int64)v625;
            goto LABEL_1082;
          }
LABEL_1068:
          v603 = *(_QWORD *)(v602 + 8);
          if ( !(*(_QWORD *)(v603 + 168) & 0x18000000LL)
            && (_UNKNOWN *)v603 != &PyCode_Type
            && (_UNKNOWN *)v603 != &PyFile_Type )
          {
            streamj = (FILE *)v599;
            v891 = PyType_IsSubtype(v603, &PyFile_Type);
            v599 = (signed __int64)streamj;
            if ( !v891 )
            {
              v7 = (sem_t *)PyExc_TypeError;
              v604 = "exec: arg 1 must be a string, file, or code object";
              PyErr_SetString(PyExc_TypeError, "exec: arg 1 must be a string, file, or code object");
              v1005 = -1;
              v16 = (__int64)streamj;
              goto LABEL_1082;
            }
          }
          stream = (FILE *)v599;
          if ( !(*(_BYTE *)(*(_QWORD *)(v601 + 8) + 171LL) & 0x20) )
          {
            v7 = (sem_t *)PyExc_TypeError;
            v604 = "exec: arg 2 must be a dictionary or None";
            PyErr_SetString(PyExc_TypeError, "exec: arg 2 must be a dictionary or None");
            v1005 = -1;
            v16 = (__int64)stream;
            goto LABEL_1082;
          }
          if ( !PyMapping_Check(v600) )
          {
            v7 = (sem_t *)PyExc_TypeError;
            v604 = "exec: arg 3 must be a mapping or None";
            PyErr_SetString(PyExc_TypeError, "exec: arg 3 must be a mapping or None");
            v1005 = -1;
            v16 = (__int64)stream;
            goto LABEL_1082;
          }
          v604 = "__builtins__";
          LODWORD(v605) = PyDict_GetItemString(v601, "__builtins__");
          v16 = (__int64)stream;
          if ( !v605 )
          {
            v604 = "__builtins__";
            PyDict_SetItemString(v601, "__builtins__", *(_QWORD *)(v914 + 40));
            v16 = (__int64)stream;
          }
          v606 = *(void **)(v602 + 8);
          if ( v606 == &PyCode_Type )
          {
            streama = (FILE *)v16;
            if ( *(_QWORD *)(*(_QWORD *)(v602 + 64) + 16LL) > 0LL )
            {
              v7 = (sem_t *)PyExc_TypeError;
              v604 = "code object passed to exec may not contain free variables";
              PyErr_SetString(PyExc_TypeError, "code object passed to exec may not contain free variables");
              v1005 = -1;
              v16 = (__int64)streama;
              goto LABEL_1082;
            }
            v604 = (const char *)v601;
            v7 = (sem_t *)v602;
            v607 = PyEval_EvalCode(v602, v601, (__int64)v600);
            v16 = (__int64)streama;
            v608 = v607;
          }
          else if ( v606 != &PyFile_Type
                 && (v604 = (const char *)&PyFile_Type,
                     v1044 = v16,
                     streamb = *(FILE **)(v602 + 8),
                     v637 = PyType_IsSubtype(*(_QWORD *)(v602 + 8), &PyFile_Type) == 0,
                     v16 = v1044,
                     v637) )
          {
            LODWORD(v1058) = 0;
            v637 = (BYTE3(streamb->__pad3) & 0x10) == 0;
            streamc = 0LL;
            if ( !v637 )
            {
              v7 = (sem_t *)v602;
              LODWORD(v893) = PyUnicodeUCS4_AsUTF8String(v602);
              v602 = (__int64)v893;
              v16 = v1044;
              if ( !v893 )
              {
LABEL_1681:
                v1005 = -1;
                goto LABEL_1082;
              }
              LODWORD(v1058) = (unsigned int)v1058 | 0x100;
              streamc = v893;
            }
            v604 = (const char *)&v1059;
            v7 = (sem_t *)v602;
            v1045 = v16;
            v615 = PyString_AsStringAndSize(v602, &v1059, 0LL);
            v16 = v1045;
            if ( v615 )
              goto LABEL_1681;
            v616 = PyEval_MergeCompilerFlags(&v1058);
            v617 = (__int64 *)&v1058;
            if ( !v616 )
              v617 = 0LL;
            v7 = (sem_t *)v1059;
            v604 = (const char *)257;
            LODWORD(v618) = PyRun_StringFlags(v1059, 257LL, v601, v600, v617);
            v608 = v618;
            v16 = v1045;
            if ( streamc )
            {
              v7 = (sem_t *)streamc;
              v619 = *(_QWORD *)&streamc->_flags - 1LL;
              *(_QWORD *)&streamc->_flags = v619;
              if ( !v619 )
              {
                (*((void (__fastcall **)(FILE *, signed __int64))streamc->_IO_read_ptr + 6))(streamc, 257LL);
                v16 = v1045;
              }
            }
          }
          else
          {
            v1047 = v16;
            LODWORD(v804) = PyFile_AsFile(v602, v604);
            streami = v804;
            LODWORD(v805) = PyFile_Name(v602);
            v7 = v805;
            LODWORD(v807) = PyString_AsString(v805, v604, v806);
            v808 = v807;
            v16 = v1047;
            if ( !v807 )
              goto LABEL_1681;
            LODWORD(v1059) = 0;
            if ( PyEval_MergeCompilerFlags(&v1059) )
              v913 = &v1059;
            else
              v913 = 0LL;
            v7 = (sem_t *)streami;
            v604 = (const char *)v808;
            v809 = PyRun_FileExFlags(streami, v601, v808, (__int64)v600, 0, (__int64)v913);
            v16 = v1047;
            v608 = v809;
          }
          if ( v1006 )
          {
            v7 = (sem_t *)v914;
            v604 = 0LL;
            v609 = v16;
            PyFrame_LocalsToFast(v914, 0LL);
            v16 = v609;
          }
          v1005 = -1;
          if ( v608 )
          {
            v1005 = 0;
            v9 = *(_QWORD *)v608 - 1LL;
            *(_QWORD *)v608 = v9;
            if ( !v9 )
            {
              v610 = (FILE *)v16;
              v7 = (sem_t *)v608;
              (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v608 + 8) + 48LL))(v608, v604);
              v16 = (__int64)v610;
            }
          }
LABEL_1082:
          v637 = (*(_QWORD *)v597)-- == 1LL;
          if ( v637 )
          {
            v611 = (FILE *)v16;
            v7 = (sem_t *)v597;
            (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v597 + 8) + 48LL))(v597, v604);
            v16 = (__int64)v611;
          }
          v612 = *(_QWORD *)v1021 - 1LL;
          *(_QWORD *)v1021 = v612;
          if ( !v612 )
          {
            v613 = (FILE *)v16;
            v7 = (sem_t *)v1021;
            (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v1021 + 8) + 48LL))(v1021, v604);
            v16 = (__int64)v613;
          }
          v10 = (signed __int64)v981;
          v4 = *v981 - 1;
          *v981 = v4;
          if ( !v4 )
          {
            v614 = v16;
            v7 = (sem_t *)v981;
            (*(void (__fastcall **)(__int64 *))(v981[1] + 48))(v981);
            v16 = v614;
          }
          goto LABEL_364;
        case 40:
        case 41:
        case 42:
        case 43:
          v589 = 0LL;
          if ( opcode & 2 )
          {
            v589 = *(_QWORD *)(v16 - 8);
            v16 -= 8LL;
          }
          v628 = 0LL;
          if ( opcode & 1 )
          {
            v628 = *(_QWORD *)(v16 - 8);
            v16 -= 8LL;
          }
          v629 = *(_QWORD *)(v16 - 16);
          v630 = *(_QWORD *)(v16 - 8);
          v592 = v628;
          v60 = v16 - 16;
          v982 = v628;
          v631 = sub_49F870(*(_QWORD *)(v16 - 8), v628, v589, *(_QWORD *)(v16 - 16));
          v637 = (*(_QWORD *)v629)-- == 1LL;
          v1005 = v631;
          v9 = v982;
          if ( v637 )
          {
            (*(void (__fastcall **)(__int64, signed __int64, __int64, _QWORD, signed __int64))(*(_QWORD *)(v629 + 8)
                                                                                             + 48LL))(
              v629,
              v592,
              v632,
              *(_QWORD *)(v629 + 8),
              v982);
            v9 = v982;
          }
          v637 = (*(_QWORD *)v630)-- == 1LL;
          if ( v637 )
          {
            v633 = v9;
            (*(void (__fastcall **)(__int64, signed __int64))(*(_QWORD *)(v630 + 8) + 48LL))(v630, v592);
            v9 = v633;
          }
          if ( v9 )
          {
            v637 = (*(_QWORD *)v9)-- == 1LL;
            if ( v637 )
              (*(void (__fastcall **)(signed __int64, signed __int64))(*(_QWORD *)(v9 + 8) + 48LL))(v9, v592);
          }
LABEL_1052:
          if ( v589 )
          {
            v637 = (*(_QWORD *)v589)-- == 1LL;
            if ( v637 )
              (*(void (__fastcall **)(__int64, signed __int64))(*(_QWORD *)(v589 + 8) + 48LL))(v589, v592);
          }
          v7 = (sem_t *)(unsigned int)v1005;
          if ( v1005 )
            goto LABEL_1054;
          goto LABEL_229;
        case 130:
          v634 = v21;
          v635 = 0LL;
          v636 = 0LL;
          v639 = __OFSUB__(v21, 1);
          v637 = v21 == 1;
          v638 = v21 - 1 < 0;
          v14 = (sem_t *)v16;
          v303 = v20;
          if ( v637 )
            goto LABEL_1124;
          if ( (unsigned __int8)(v638 ^ v639) | v637 )
          {
            v641 = v16;
            if ( v634 )
              goto LABEL_1222;
          }
          else
          {
            if ( v634 != 2 )
            {
              if ( v634 != 3 )
              {
LABEL_1222:
                a2 = "bad RAISE_VARARGS oparg";
                next_instr = v303;
                PyErr_SetString(PyExc_SystemError, "bad RAISE_VARARGS oparg");
                v10 = 2LL;
                goto LABEL_911;
              }
              v14 = (sem_t *)(v16 - 8);
              v635 = *(_QWORD *)(v16 - 8);
            }
            v636 = (__int64 *)*(&v14[-1].__align + 3);
            v14 = (sem_t *)((char *)v14 - 8);
LABEL_1124:
            v640 = *(&v14[-1].__align + 3);
            v641 = (signed __int64)(&v14[-1].__align + 3);
            v1055 = v636;
            v1056 = v635;
            v1054 = v640;
            if ( v640 )
              goto LABEL_1125;
          }
          v647 = &Py_NoneStruct;
          v648 = (__int64 *)*((_QWORD *)PyThreadState_Current + 12);
          v4 = *((_QWORD *)PyThreadState_Current + 13);
          v649 = *((_QWORD *)PyThreadState_Current + 14);
          v1055 = (__int64 *)*((_QWORD *)PyThreadState_Current + 13);
          if ( v648 )
            v647 = v648;
          v1056 = v649;
          v1054 = (__int64)v647;
          ++*v647;
          if ( v1055 )
            ++*v1055;
          if ( !v1056 )
            goto LABEL_1127;
          ++*(_QWORD *)v1056;
          v635 = v1056;
LABEL_1125:
          if ( (__int64 *)v635 == &Py_NoneStruct )
          {
            if ( !--Py_NoneStruct )
              (*((void (__fastcall **)(_QWORD))off_920AB8 + 6))(&Py_NoneStruct);
            v1056 = 0LL;
          }
          else if ( v635 && *(_UNKNOWN **)(v635 + 8) != &PyTraceBack_Type )
          {
            a2 = "raise: arg 3 must be a traceback or None";
            PyErr_SetString(PyExc_TypeError, "raise: arg 3 must be a traceback or None");
            goto LABEL_1147;
          }
LABEL_1127:
          if ( !v1055 )
          {
            ++Py_NoneStruct;
            v1055 = &Py_NoneStruct;
          }
          while ( 1 )
          {
            v642 = (__int64 *)v1054;
            v643 = *(_QWORD *)(v1054 + 8);
            if ( !(*(_BYTE *)(v643 + 171) & 4) )
              break;
            LODWORD(v909) = PyTuple_Size(v1054);
            v642 = (__int64 *)v1054;
            if ( v909 <= 0 )
            {
              v643 = *(_QWORD *)(v1054 + 8);
              break;
            }
            v1054 = *(_QWORD *)(v1054 + 24);
            ++*(_QWORD *)v1054;
            v637 = (*v642)-- == 1;
            if ( v637 )
              (*(void (**)(void))(v642[1] + 48))();
          }
          if ( (_UNKNOWN *)v643 == &PyClass_Type
            || (v644 = *(_QWORD *)(v643 + 168), v644 & 0x80000000) && *((_BYTE *)v642 + 171) & 0x40 )
          {
            PyErr_NormalizeException(&v1054, &v1055, &v1056);
            v646 = v1055[1];
            if ( (_UNKNOWN *)v646 != &PyInstance_Type && !(*(_BYTE *)(v646 + 171) & 0x40) )
            {
              a2 = "calling %s() should have returned an instance of BaseException, not '%s'";
              PyErr_Format(
                PyExc_TypeError,
                "calling %s() should have returned an instance of BaseException, not '%s'",
                *(_QWORD *)(v1054 + 24),
                *(_QWORD *)(v646 + 24));
              goto LABEL_1147;
            }
          }
          else
          {
            if ( (_UNKNOWN *)v643 != &PyInstance_Type && !(v644 & 0x40000000) )
            {
              a2 = "exceptions must be old-style classes or derived from BaseException, not %s";
              PyErr_Format(
                PyExc_TypeError,
                "exceptions must be old-style classes or derived from BaseException, not %s",
                *(_QWORD *)(v643 + 24),
                v4);
              goto LABEL_1147;
            }
            if ( v1055 != &Py_NoneStruct )
            {
              a2 = "instance exception may not have a separate value";
              PyErr_SetString(PyExc_TypeError, "instance exception may not have a separate value");
              goto LABEL_1147;
            }
            if ( !--Py_NoneStruct )
            {
              (*((void (__fastcall **)(_QWORD))off_920AB8 + 6))(&Py_NoneStruct);
              v642 = (__int64 *)v1054;
              v643 = *(_QWORD *)(v1054 + 8);
            }
            v1055 = v642;
            if ( (_UNKNOWN *)v643 == &PyInstance_Type )
              v643 = v642[2];
            v1054 = v643;
            ++*(_QWORD *)v643;
          }
          v645 = v1054;
          if ( !Py_Py3kWarningFlag || *(_UNKNOWN **)(v1054 + 8) != &PyClass_Type )
            goto LABEL_1141;
          a2 = "exceptions must derive from BaseException in 3.x";
          if ( PyErr_WarnEx(PyExc_DeprecationWarning, "exceptions must derive from BaseException in 3.x", 1LL) >= 0 )
          {
            v645 = v1054;
LABEL_1141:
            a2 = (const char *)v1055;
            PyErr_Restore(v645, v1055, v1056);
            v10 = v1056 < 1 ? 2 : 4;
            goto LABEL_1142;
          }
LABEL_1147:
          v9 = (signed __int64)v1055;
          if ( v1055 )
          {
            v637 = (*v1055)-- == 1;
            if ( v637 )
              (*(void (__fastcall **)(__int64 *, const char *))(v1055[1] + 48))(v1055, a2);
          }
          if ( v1054 )
          {
            v637 = (*(_QWORD *)v1054)-- == 1LL;
            if ( v637 )
              (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v1054 + 8) + 48LL))(v1054, a2);
          }
          v10 = 2LL;
          if ( v1056 )
          {
            a2 = *(const char **)v1056;
            v637 = (*(_QWORD *)v1056)-- == 1LL;
            if ( v637 )
            {
              (*(void (**)(void))(*(_QWORD *)(v1056 + 8) + 48LL))();
              v10 = 2LL;
            }
          }
LABEL_1142:
          v14 = (sem_t *)v641;
LABEL_496:
          next_instr = v303;
          v7 = v14;
          if ( (_DWORD)v10 == 2 || (_DWORD)v10 == 4 )
            goto LABEL_497;
          goto LABEL_806;
        case 74:
          v650 = v16;
          v16 -= 8LL;
          v584 = *(_QWORD *)(v650 - 8);
          if ( v584 )
          {
            v586 = *(_QWORD *)(v650 - 8);
            if ( (__int64 *)v584 != &Py_NoneStruct )
              goto LABEL_1035;
          }
LABEL_1034:
          v979 = v16;
          LODWORD(v585) = PySys_GetObject("stdout");
          v586 = v585;
          v16 = v979;
          if ( v585 )
          {
LABEL_1035:
            ++*(_QWORD *)v586;
            a2 = (const char *)v586;
            v7 = (sem_t *)"\n";
            v980 = v16;
            v1005 = PyFile_WriteString("\n");
            v16 = v980;
            if ( !v1005 )
            {
              a2 = 0LL;
              v7 = (sem_t *)v586;
              PyFile_SoftSpace(v586, 0LL);
              v16 = v980;
            }
            v302 = 1;
            v637 = (*(_QWORD *)v586)-- == 1LL;
            if ( v637 )
            {
              v587 = v16;
              v7 = (sem_t *)v586;
              (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v586 + 8) + 48LL))(v586, a2);
              v16 = v587;
            }
          }
          else
          {
            v7 = (sem_t *)PyExc_RuntimeError;
            a2 = "lost sys.stdout";
            v302 = 2;
            PyErr_SetString(PyExc_RuntimeError, "lost sys.stdout");
            v16 = v979;
          }
          if ( v584 )
          {
            v637 = (*(_QWORD *)v584)-- == 1LL;
            if ( v637 )
            {
              v588 = v16;
              v7 = (sem_t *)v584;
              (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v584 + 8) + 48LL))(v584, a2);
              v16 = v588;
            }
          }
          goto LABEL_494;
        case 79:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlaceOr(v7, *(_QWORD *)(v16 - 8));
          goto LABEL_394;
        case 65:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v205 = *(_QWORD *)(v16 - 16);
          v986 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v661) = sub_561E20(v7, *(_QWORD *)(v16 - 8), 120LL);
          v15 = v661;
          v16 = v986;
          if ( v661 == &Py_NotImplementedStruct )
          {
            if ( !--Py_NotImplementedStruct )
            {
              (*((void (__fastcall **)(_QWORD))off_9056A8 + 6))(&Py_NotImplementedStruct);
              v16 = v986;
            }
            v937 = v16;
            v237 = *(_QWORD *)(*(_QWORD *)(v205 + 8) + 24LL);
            v662 = *(_QWORD *)(*(_QWORD *)(v206 + 8) + 24LL);
            v236 = "^";
LABEL_392:
            v7 = (sem_t *)PyExc_TypeError;
            v15 = 0LL;
            PyErr_Format(PyExc_TypeError, "unsupported operand type(s) for %.100s: '%.100s' and '%.100s'", v236, v237);
            v16 = v937;
          }
          goto LABEL_319;
        case 15:
          v22 = *(_QWORD *)(v16 - 8);
          if ( v22 )
          {
            v663 = *(_QWORD *)(v22 + 8);
            v664 = *(_QWORD *)(v663 + 96);
            if ( v664 && (v310 = *(int (__fastcall **)(__int64))(v664 + 88)) != 0LL )
            {
LABEL_510:
              v311 = v16;
              v7 = (sem_t *)v22;
              LODWORD(v312) = v310(v22);
              v16 = v311;
              v15 = v312;
            }
            else
            {
              v665 = *(_QWORD *)(v663 + 24);
              v987 = v16;
              v666 = "bad operand type for unary ~: '%.200s'";
LABEL_1195:
              v7 = (sem_t *)PyExc_TypeError;
              v15 = 0LL;
              PyErr_Format(PyExc_TypeError, v666, v665, v4);
              v16 = v987;
            }
          }
          else
          {
LABEL_1548:
            v834 = v16;
            LODWORD(v835) = sub_455D56();
            v16 = v834;
            v15 = v835;
          }
          v637 = (*(_QWORD *)v22)-- == 1LL;
          if ( !v637 )
            goto LABEL_18;
LABEL_512:
          v952 = v16;
          goto LABEL_513;
        case 77:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlaceAnd(v7, *(_QWORD *)(v16 - 8));
          goto LABEL_394;
        case 71:
          v575 = v16 - 8;
          v576 = *(_QWORD *)(v16 - 8);
          v574 = 0LL;
LABEL_1006:
          LODWORD(v577) = PySys_GetObject("stdout");
          v578 = v577;
          if ( !v577 )
          {
            v579 = "lost sys.stdout";
            PyErr_SetString(PyExc_RuntimeError, "lost sys.stdout");
            v1005 = -1;
            goto LABEL_1018;
          }
LABEL_1007:
          ++*(_QWORD *)v578;
          v579 = 0LL;
          if ( PyFile_SoftSpace(v578, 0LL) )
          {
            v579 = (const char *)v578;
            v1005 = PyFile_WriteString(&unk_6059E8);
          }
          if ( v1005 )
            goto LABEL_1015;
          v579 = (const char *)v578;
          v1005 = PyFile_WriteObject(v576, v578, 1LL);
          if ( v1005 )
            goto LABEL_1015;
          v580 = *(_QWORD *)(*(_QWORD *)(v576 + 8) + 168LL);
          if ( v580 & 0x8000000 )
          {
            v1020 = v576 + 36;
            v978 = *(_QWORD *)(v576 + 16);
            if ( !*(_QWORD *)(v576 + 16) )
              goto LABEL_1014;
            v581 = __ctype_b_loc();
            v4 = v576 + 36;
            v9 = *(_BYTE *)(v1020 + v978 - 1);
            if ( !(HIBYTE((*v581)[v9]) & 0x20) || *(_BYTE *)(v1020 + v978 - 1) == 32 )
              goto LABEL_1014;
          }
          else
          {
            if ( !(v580 & 0x10000000) || (v10 = *(_QWORD *)(v576 + 16), v9 = *(_QWORD *)(v576 + 24), !v10) )
            {
LABEL_1014:
              v579 = (const char *)1;
              PyFile_SoftSpace(v578, 1LL);
              goto LABEL_1015;
            }
            v583 = *(_DWORD *)(v9 + 4 * v10 - 4);
            if ( (unsigned int)v583 > 0x7F )
            {
              if ( !PyUnicodeUCS4_IsWhitespace(v583, v578) )
                goto LABEL_1014;
            }
            else
            {
              v579 = (const char *)(unsigned int)v583;
              if ( !Py_ascii_whitespace[(unsigned __int64)(unsigned int)v583] || (_DWORD)v583 == 32 )
                goto LABEL_1014;
            }
          }
LABEL_1015:
          if ( v578 )
          {
            v637 = (*(_QWORD *)v578)-- == 1LL;
            if ( v637 )
              (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v578 + 8) + 48LL))(v578, v579);
          }
LABEL_1018:
          v637 = (*(_QWORD *)v576)-- == 1LL;
          v16 = v575;
          if ( v637 )
          {
            (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v576 + 8) + 48LL))(v576, v579);
            v16 = v575;
          }
          if ( v574 )
          {
            v637 = (*(_QWORD *)v574)-- == 1LL;
            if ( v637 )
            {
              v582 = v16;
              (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v574 + 8) + 48LL))(v574, v579);
              v16 = v582;
            }
          }
          goto LABEL_233;
        case 91:
          v988 = v16;
          v675 = *(_QWORD *)(v1004 + 8LL * v21 + 24);
          v15 = *(__int64 **)(v914 + 56);
          if ( v15 )
          {
            v7 = *(sem_t **)(v914 + 56);
            v1005 = PyObject_DelItem(v7, *(_QWORD *)(v1004 + 8LL * v21 + 24));
            v16 = v988;
            if ( v1005 )
            {
              v7 = (sem_t *)PyExc_NameError;
              sub_44DD3B(PyExc_NameError, "name '%.200s' is not defined", v675);
              v16 = v988;
            }
          }
          else
          {
            LODWORD(v787) = PyObject_Repr(*(_QWORD *)(v1004 + 8LL * v21 + 24));
            v7 = (sem_t *)PyExc_SystemError;
            PyErr_Format(PyExc_SystemError, "no locals when deleting %s", v787 + 36, v788);
            v16 = v988;
          }
          goto LABEL_364;
        case 119:
          v7 = (sem_t *)v21;
          v989 = (sem_t *)v16;
          LODWORD(v676) = PyInt_FromLong(v21);
          v1010 = v676;
          v16 = (__int64)v989;
          if ( !v676 )
            goto LABEL_1543;
          v62 = v15;
          PyFrameObject = v914;
          v7 = v989;
          v63 = 32;
          goto LABEL_68;
        case 97:
          v973 = v16 - 8;
          v146 = *(_QWORD *)(v16 - 8);
          v691 = PyDict_SetItem(*(_QWORD *)(v914 + 48), *(_QWORD *)(v1004 + 8LL * v21 + 24), *(_QWORD *)(v16 - 8));
          v637 = (*(_QWORD *)v146)-- == 1LL;
          v1005 = v691;
          v16 = v973;
          if ( v637 )
            goto LABEL_958;
          goto LABEL_233;
        case 96:
          v692 = *(_QWORD *)(v16 - 8);
          v990 = v16 - 8;
          v7 = *(sem_t **)(v16 - 8);
          v693 = PyObject_SetAttr(v7, *(_QWORD *)(v1004 + 8LL * v21 + 24), 0LL);
          v637 = (*(_QWORD *)v692)-- == 1LL;
          v1005 = v693;
          v16 = v990;
          if ( v637 )
          {
            v7 = (sem_t *)v692;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v692 + 8) + 48LL))(v692);
            v16 = v990;
          }
          goto LABEL_364;
        case 84:
          v1022 = v16 - 8;
          v991 = *(_QWORD *)(v16 - 8);
          PyFrame_FastToLocals(v914);
          v15 = *(__int64 **)(v914 + 56);
          if ( !v15 )
          {
            v7 = (sem_t *)PyExc_SystemError;
            PyErr_SetString(PyExc_SystemError, "no locals found during 'import *'");
            v16 = v1022;
            goto LABEL_364;
          }
          LODWORD(v694) = PyObject_GetAttrString(v991, "__all__");
          v695 = v694;
          v696 = (FILE *)v1022;
          if ( !v694 )
          {
            v697 = PyExc_AttributeError;
            v698 = PyErr_ExceptionMatches(PyExc_AttributeError, "__all__");
            v696 = (FILE *)v1022;
            if ( v698 )
            {
              PyErr_Clear(v697, "__all__");
              LODWORD(v699) = PyObject_GetAttrString(v991, "__dict__");
              v700 = v699;
              if ( v699 )
              {
                v701 = PyObject_CallMethod(v699);
                v637 = (*(_QWORD *)v700)-- == 1LL;
                v695 = v701;
                v696 = (FILE *)v1022;
                if ( v637 )
                {
                  (*(void (__fastcall **)(__int64, const char *))(*(_QWORD *)(v700 + 8) + 48LL))(v700, "keys");
                  v696 = (FILE *)v1022;
                }
                if ( v695 )
                {
                  streamd = 1;
                  goto LABEL_1270;
                }
              }
              else
              {
                v798 = PyErr_ExceptionMatches(PyExc_AttributeError, "__dict__");
                v696 = (FILE *)v1022;
                if ( v798 )
                {
                  PyErr_SetString(PyExc_ImportError, "from-import-* object has no __dict__ and no __all__");
                  v1005 = -1;
                  v696 = (FILE *)v1022;
                  goto LABEL_1318;
                }
              }
            }
            v1005 = -1;
            goto LABEL_1318;
          }
          streamd = 0;
LABEL_1270:
          v702 = *(_QWORD *)(v695 + 8);
          v703 = *(_QWORD *)(v702 + 104);
          if ( v703 )
          {
            v704 = *(int (__fastcall **)(__int64, const char *))(v703 + 24);
            if ( v704 )
            {
              v1046 = 0;
              v705 = 0LL;
              v1007 = v696;
              v1023 = v20;
              v1053 = v15 - 4;
              while ( 1 )
              {
                a2 = v705;
                LODWORD(v710) = v704(v695, v705);
                v711 = v710;
                if ( !v710 )
                {
LABEL_1313:
                  v726 = v1007;
                  v20 = v1023;
                  goto LABEL_1314;
                }
                if ( streamd )
                {
                  v712 = *(_QWORD *)(v710 + 8);
                  if ( *(_BYTE *)(v712 + 171) & 8 )
                  {
                    if ( *(_BYTE *)(v710 + 36) == 95 )
                    {
                      v637 = (*(_QWORD *)v710)-- == 1LL;
                      if ( v637 )
                        (*(void (__fastcall **)(__int64))(v712 + 48))(v710);
                      goto LABEL_1286;
                    }
                  }
                }
                a2 = (const char *)v710;
                LODWORD(v713) = PyObject_GetAttr(v991, v710);
                v714 = v713;
                if ( !v713 )
                  goto LABEL_1459;
                if ( (_UNKNOWN *)v15[1] != &PyDict_Type )
                {
                  a2 = (const char *)v711;
                  LODWORD(v708) = PyObject_SetItem(v15, v711, v713);
                  goto LABEL_1280;
                }
                if ( !(byte_91E9AB & 0x20) )
                {
                  a2 = (const char *)803;
                  LODWORD(v708) = -1;
                  PyErr_BadInternalCall("../Objects/dictobject.c", 803LL);
                  goto LABEL_1280;
                }
                if ( *(_UNKNOWN **)(v711 + 8) == &PyString_Type )
                {
                  v708 = *(_QWORD *)(v711 + 24);
                  if ( v708 == -1LL )
                  {
                    LODWORD(v715) = PyObject_Hash(v711);
                    v708 = v715;
                  }
                }
                else
                {
                  LODWORD(v732) = PyObject_Hash(v711);
                  v708 = v732;
                  if ( v732 == -1LL )
                    goto LABEL_1280;
                }
                v716 = v15[3];
                ++*(_QWORD *)v714;
                ++*(_QWORD *)v711;
                v717 = (__int64 (__fastcall *)())v15[6];
                v1052 = v716;
                if ( v717 != sub_5324C0 )
                {
                  a2 = (const char *)v711;
                  LODWORD(v731) = ((int (__fastcall *)(__int64 *, __int64, unsigned __int64))v717)(v15, v711, v708);
                  v719 = v731;
                  goto LABEL_1303;
                }
                a2 = (const char *)v15[4];
                v1048 = v15[4];
                v1049 = v15[5];
                if ( *(_UNKNOWN **)(v711 + 8) != &PyString_Type )
                {
                  v15[6] = (__int64)sub_532210;
                  a2 = (const char *)v711;
                  LODWORD(v797) = sub_532210(v15, v711, v708);
                  v719 = v797;
                  goto LABEL_1303;
                }
                v1050 = v708;
                v718 = v708 & v1048;
                v719 = v15[5] + 24 * (v708 & v1048);
                v720 = *(_QWORD *)(v719 + 8);
                if ( v720 && v711 != v720 )
                {
                  v1051 = qword_9333F0;
                  if ( v720 != qword_9333F0 )
                  {
                    if ( v708 == *(_QWORD *)v719 )
                    {
                      v794 = *(_QWORD *)(v720 + 16);
                      if ( v794 == *(_QWORD *)(v711 + 16) )
                      {
                        a2 = (const char *)(v711 + 36);
                        v795 = memcmp((const void *)(v720 + 36), (const void *)(v711 + 36), v794);
                        v718 = v708 & v1048;
                        if ( !v795 )
                          goto LABEL_1303;
                      }
                    }
                    v719 = 0LL;
                  }
                  v733 = v719;
                  while ( 2 )
                  {
                    v718 += v1050 + 1 + 4 * v718;
                    a2 = (const char *)(v718 & v1048);
                    v719 = v1049 + 24 * (v718 & v1048);
                    v734 = *(_QWORD *)(v719 + 8);
                    if ( !v734 )
                    {
                      if ( v733 )
                        v719 = v733;
                      break;
                    }
                    if ( v711 == v734 )
                      break;
                    if ( v708 == *(_QWORD *)v719 )
                    {
                      if ( v1051 != v734 )
                      {
                        v791 = *(_QWORD *)(v734 + 16);
                        if ( v791 == *(_QWORD *)(v711 + 16) )
                        {
                          a2 = (const char *)(v711 + 36);
                          v792 = v718;
                          v793 = memcmp((const void *)(v734 + 36), (const void *)(v711 + 36), v791);
                          v718 = v792;
                          if ( !v793 )
                            break;
                        }
                        goto LABEL_1331;
                      }
                    }
                    else if ( v1051 != v734 )
                    {
LABEL_1331:
                      v1050 >>= 5;
                      continue;
                    }
                    break;
                  }
                  if ( !v733 )
                    v733 = v1049 + 24 * (v718 & v1048);
                  goto LABEL_1331;
                }
LABEL_1303:
                if ( !v719 )
                {
                  v637 = (*(_QWORD *)v711)-- == 1LL;
                  if ( v637 )
                    (*(void (__fastcall **)(__int64))(*(_QWORD *)(v711 + 8) + 48LL))(v711);
                  v637 = (*(_QWORD *)v714)-- == 1LL;
                  if ( v637 )
                    (*(void (__fastcall **)(__int64))(*(_QWORD *)(v714 + 8) + 48LL))(v714);
LABEL_1459:
                  LODWORD(v708) = -1;
                  goto LABEL_1280;
                }
                if ( *(v15 - 2) == -2 )
                {
                  if ( (v721 = *(_QWORD *)(v711 + 8), *(_BYTE *)(v721 + 169) & 0x40)
                    && ((v736 = *(int (__fastcall **)(__int64))(v721 + 328)) == 0LL || v736(v711))
                    && (*(_UNKNOWN **)(v711 + 8) != &PyTuple_Type || *(_QWORD *)(v711 - 16) != -2LL)
                    || (v722 = *(_QWORD *)(v714 + 8), *(_BYTE *)(v722 + 169) & 0x40)
                    && ((v723 = *(int (__fastcall **)(__int64))(v722 + 328)) == 0LL || v723(v714))
                    && (*(_UNKNOWN **)(v714 + 8) != &PyTuple_Type || *(_QWORD *)(v714 - 16) != -2LL) )
                  {
                    if ( *(v15 - 2) != -2 )
                      goto LABEL_1526;
                    *(v15 - 2) = -3LL;
                    a2 = (const char *)(v15 - 4);
                    *(v15 - 4) = (__int64)PyGC_generation0;
                    v724 = (__int64 **)PyGC_generation0[1];
                    *(v15 - 3) = (__int64)v724;
                    *v724 = v1053;
                    PyGC_generation0[1] = v1053;
                  }
                }
                v706 = *(_QWORD *)(v719 + 16);
                if ( v706 )
                {
                  *(_QWORD *)(v719 + 16) = v714;
                  v637 = (*(_QWORD *)v706)-- == 1LL;
                  if ( v637 )
                  {
                    v737 = *(void (**)(void))(*(_QWORD *)(v706 + 8) + 48LL);
                    if ( (char *)v737 == (char *)sub_55F260 )
                      sub_55F260();
                    else
                      v737();
                  }
                  v637 = (*(_QWORD *)v711)-- == 1LL;
                  if ( v637 )
                  {
                    (*(void (__fastcall **)(__int64))(*(_QWORD *)(v711 + 8) + 48LL))(v711);
                    v707 = v15[3];
                  }
                  else
                  {
                    v707 = v15[3];
                  }
                }
                else
                {
                  if ( *(_QWORD *)(v719 + 8) )
                  {
                    v725 = qword_9333F0;
                    v637 = *(_QWORD *)qword_9333F0 == 1LL;
                    --*(_QWORD *)v725;
                    if ( v637 )
                      (*(void (**)(void))(*(_QWORD *)(v725 + 8) + 48LL))();
                  }
                  else
                  {
                    ++v15[2];
                  }
                  *(_QWORD *)(v719 + 8) = v711;
                  *(_QWORD *)v719 = v708;
                  *(_QWORD *)(v719 + 16) = v714;
                  v707 = v15[3] + 1;
                  v15[3] = v707;
                }
                LODWORD(v708) = 0;
                if ( v1052 < v707 && 3 * v15[2] >= 2 * v15[4] + 2 )
                {
                  a2 = (const char *)(v707 * (2LL * (v707 < 50001) + 2));
                  LODWORD(v708) = sub_54E4A0(v15, a2);
                }
LABEL_1280:
                v637 = (*(_QWORD *)v711)-- == 1LL;
                if ( v637 )
                  (*(void (__fastcall **)(__int64))(*(_QWORD *)(v711 + 8) + 48LL))(v711);
                if ( v714 )
                {
                  v637 = (*(_QWORD *)v714)-- == 1LL;
                  if ( v637 )
                    (*(void (__fastcall **)(__int64))(*(_QWORD *)(v714 + 8) + 48LL))(v714);
                }
                if ( (_DWORD)v708 )
                {
                  v696 = v1007;
                  v20 = v1023;
                  v1005 = v708;
                  goto LABEL_1316;
                }
LABEL_1286:
                v702 = *(_QWORD *)(v695 + 8);
                v705 = (const char *)++v1046;
                v709 = *(_QWORD *)(v702 + 104);
                if ( !v709 || (v704 = *(int (__fastcall **)(__int64, const char *))(v709 + 24)) == 0LL )
                {
                  v696 = v1007;
                  v20 = v1023;
                  break;
                }
                if ( v1046 < 0LL && *(_QWORD *)v709 )
                {
                  LODWORD(v735) = (*(int (__fastcall **)(__int64))v709)(v695);
                  if ( v735 < 0 )
                    goto LABEL_1313;
                  v705 = (const char *)(v735 + v1046);
                  v704 = *(int (__fastcall **)(__int64, const char *))(v709 + 24);
                }
              }
            }
          }
          v796 = v696;
          a2 = "'%.200s' object does not support indexing";
          PyErr_Format(PyExc_TypeError, "'%.200s' object does not support indexing", *(_QWORD *)(v702 + 24), v702);
          v726 = v796;
LABEL_1314:
          v727 = PyExc_IndexError;
          v1024 = v726;
          v728 = PyErr_ExceptionMatches(PyExc_IndexError, a2);
          v1005 = -1;
          v696 = v1024;
          if ( v728 )
          {
            PyErr_Clear(v727, a2);
            v696 = v1024;
            v1005 = 0;
          }
LABEL_1316:
          v637 = (*(_QWORD *)v695)-- == 1LL;
          if ( v637 )
          {
            v729 = v696;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v695 + 8) + 48LL))(v695);
            v696 = v729;
          }
LABEL_1318:
          streame = v696;
          PyFrame_LocalsToFast(v914, 0LL);
          v16 = (__int64)streame;
          v730 = *(_QWORD *)v991 - 1LL;
          *(_QWORD *)v991 = v730;
          if ( !v730 )
          {
            (*(void (__fastcall **)(__int64, _QWORD))(*(_QWORD *)(v991 + 8) + 48LL))(v991, 0LL);
            v16 = (__int64)streame;
          }
LABEL_233:
          v7 = (sem_t *)(unsigned int)v1005;
          if ( !v1005 )
            goto LABEL_19;
          goto LABEL_364;
        case 89:
          v738 = *(_QWORD *)(v16 - 8);
          v739 = *(_QWORD *)(v16 - 16);
          v740 = *(_QWORD *)(v16 - 24);
          v992 = v16;
          v741 = v16 - 16;
          if ( *(_BYTE *)(*(_QWORD *)(v738 + 8) + 171LL) & 0x20 )
          {
            v742 = v741;
            LODWORD(v743) = PyDict_GetItemString(v738, "__metaclass__");
            v744 = v743;
            v741 = v742;
            if ( v743 )
              goto LABEL_1374;
          }
          if ( *(_BYTE *)(*(_QWORD *)(v739 + 8) + 171LL) & 4 && *(_QWORD *)(v739 + 16) > 0LL )
          {
            v745 = *(_QWORD *)(v739 + 24);
            v1025 = v741;
            v746 = *(_QWORD *)(v739 + 24);
            LODWORD(v747) = PyObject_GetAttrString(v746, "__class__");
            v744 = v747;
            v741 = v1025;
            if ( !v747 )
            {
              PyErr_Clear(v746, "__class__");
              v744 = *(sem_t **)(v745 + 8);
              v741 = v1025;
              ++v744->__align;
            }
          }
          else
          {
            v1026 = v741;
            LODWORD(v751) = PyThreadState_GetFrame(PyThreadState_Current);
            v741 = v1026;
            if ( !v751
              || (v752 = *(_QWORD *)(v751 + 48)) == 0
              || !(*(_BYTE *)(*(_QWORD *)(v752 + 8) + 171LL) & 0x20)
              || (LODWORD(v753) = PyDict_GetItemString(v752, "__metaclass__"), v744 = v753, v741 = v1026, !v753) )
            {
              v744 = (sem_t *)&PyClass_Type;
            }
LABEL_1374:
            ++v744->__align;
          }
          streamf = (FILE *)v741;
          v7 = v744;
          v15 = (__int64 *)PyObject_CallFunctionObjArgs((char)v744);
          v16 = (__int64)streamf;
          v637 = v7->__align-- == 1;
          if ( v637 )
          {
            (*(void (__fastcall **)(sem_t *, __int64))(*(&v7->__align + 1) + 48))(v7, v740);
            v16 = (__int64)streamf;
          }
          if ( !v15 )
          {
            v7 = (sem_t *)PyExc_TypeError;
            v1029 = v16;
            v777 = PyErr_ExceptionMatches(PyExc_TypeError, v740);
            v16 = v1029;
            if ( v777 )
            {
              PyErr_Fetch(&v1057, &v1058, &v1059);
              v778 = v1058;
              v779 = v1029;
              if ( *(_BYTE *)(v1058[1] + 171) & 8 )
              {
                v780 = (__int64)v1058 + 36;
                v781 = (__int64 *)PyString_FromFormat((unsigned __int64)"Error when calling the metaclass bases\n    %s");
                v779 = v1029;
                if ( v781 )
                {
                  v637 = (*v1058)-- == 1;
                  if ( v637 )
                  {
                    (*(void (__fastcall **)(__int64 *, __int64))(v1058[1] + 48))(v1058, v780);
                    v779 = v1029;
                  }
                  v1058 = v781;
                  v778 = v781;
                }
                else
                {
                  v778 = v1058;
                }
              }
              v7 = (sem_t *)v1057;
              v782 = v779;
              PyErr_Restore(v1057, v778, v1059);
              v16 = v782;
            }
          }
          *(_QWORD *)(v992 - 24) = v15;
          v637 = (*(_QWORD *)v738)-- == 1LL;
          if ( v637 )
          {
            v748 = v16;
            v7 = (sem_t *)v738;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v738 + 8) + 48LL))(v738);
            v16 = v748;
          }
          v637 = (*(_QWORD *)v739)-- == 1LL;
          if ( v637 )
          {
            v749 = v16;
            v7 = (sem_t *)v739;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v739 + 8) + 48LL))(v739);
            v16 = v749;
          }
          v637 = (*(_QWORD *)v740)-- == 1LL;
          if ( v637 )
          {
            v750 = v16;
            v7 = (sem_t *)v740;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v740 + 8) + 48LL))(v740);
            v16 = v750;
          }
          goto LABEL_364;
        case 28:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlaceFloorDivide(v7, *(_QWORD *)(v16 - 8));
          goto LABEL_394;
        case 145:
          opcode = *(_BYTE *)v20;
          v20 += 3LL;
          v7 = (sem_t *)(unsigned int)(v21 << 16);
          a2 = (const char *)((unsigned int)*(_BYTE *)(v20 - 1) << 8);
          v21 = (unsigned int)v7 | ((_DWORD)a2 + *(_BYTE *)(v20 - 2));
          goto LABEL_14;
        case 67:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlacePower(v7, *(_QWORD *)(v16 - 8), &Py_NoneStruct);
          goto LABEL_394;
        case 29:
LABEL_1556:
          v840 = *(_QWORD *)(v16 - 16);
          v841 = *(_QWORD *)(v16 - 8);
          v350 = v16 - 8;
          v999 = (sem_t *)v16;
          LODWORD(v842) = PyNumber_InPlaceTrueDivide(*(_QWORD *)(v16 - 16), *(_QWORD *)(v16 - 8));
          v637 = (*(_QWORD *)v840)-- == 1LL;
          v15 = v842;
          v7 = v999;
          if ( v637 )
          {
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v840 + 8) + 48LL))(v840);
            v7 = v999;
          }
          v637 = (*(_QWORD *)v841)-- == 1LL;
          if ( v637 )
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v841 + 8) + 48LL))(v841);
LABEL_561:
          *(&v7[-1].__align + 2) = (__int64)v15;
          v16 = v350;
          if ( !v15 )
            goto LABEL_364;
          goto LABEL_19;
        case 98:
          v1000 = v16;
          v843 = *(_QWORD *)(v1004 + 8LL * v21 + 24);
          v7 = *(sem_t **)(v914 + 48);
          v1005 = PyDict_DelItem(v7, v843);
          v16 = v1000;
          if ( v1005 )
          {
            v7 = (sem_t *)PyExc_NameError;
            sub_44DD3B(PyExc_NameError, "global name '%.200s' is not defined", v843);
            v16 = v1000;
          }
          goto LABEL_364;
        case 147:
          v91 = *(_QWORD *)(v16 - 8);
          v844 = *(_QWORD *)(v16 - 16);
          v1001 = v16 - 16;
          v845 = PyDict_SetItem(*(_QWORD *)(v16 + 8LL * -v21 - 16), v91, *(_QWORD *)(v16 - 16));
          v637 = (*(_QWORD *)v844)-- == 1LL;
          v1005 = v845;
          v16 = v1001;
          if ( v637 )
          {
            (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v844 + 8) + 48LL))(v844, v91);
            v16 = v1001;
          }
          v637 = (*(_QWORD *)v91)-- == 1LL;
          if ( v637 )
          {
            v925 = v16;
LABEL_1566:
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v91 + 8) + 48LL))(v91);
            v16 = v925;
          }
LABEL_125:
          v7 = (sem_t *)(unsigned int)v1005;
          if ( v1005 )
            goto LABEL_364;
          if ( *(_BYTE *)v20 != 113 )
            goto LABEL_19;
          v1005 = 0;
          v21 = (*(_BYTE *)(v20 + 2) << 8) + *(_BYTE *)(v20 + 1);
LABEL_128:
          next_instr = v918 + v21;
          goto LABEL_10;
        case 13:
          v22 = *(_QWORD *)(v16 - 8);
          v952 = v16;
          v7 = *(sem_t **)(v16 - 8);
          LODWORD(v538) = PyObject_Repr(v7);
LABEL_925:
          v637 = (*(_QWORD *)v22)-- == 1LL;
          v15 = v538;
          v16 = v952;
          if ( v637 )
          {
LABEL_513:
            v7 = (sem_t *)v22;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v22 + 8) + 48LL))(v22);
            v16 = v952;
          }
          goto LABEL_18;
        case 5:
          v868 = *(_QWORD *)(v16 - 24);
          next_instr = v20;
          v15 = *(__int64 **)(v16 - 32);
          v9 = *(_QWORD *)(v16 - 16);
          *(_QWORD *)(v16 - 32) = *(_QWORD *)(v16 - 8);
          *(_QWORD *)(v16 - 16) = v868;
          *(_QWORD *)(v16 - 8) = v9;
          *(_QWORD *)(v16 - 24) = v15;
          continue;
        default:
          v14 = (sem_t *)v16;
          next_instr = v20;
          v869 = PyFrame_GetLineNumber(v914);
          __fprintf_chk(stderr, 1LL, "XXX lineno: %d, opcode: %d\n", (unsigned int)v869);
          a2 = "unknown opcode";
          PyErr_SetString(PyExc_SystemError, "unknown opcode");
          v10 = 2LL;
          goto LABEL_911;
        case 59:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlaceRemainder(v7, *(_QWORD *)(v16 - 8));
          goto LABEL_394;
        case 78:
          v203 = v16;
          v206 = *(_QWORD *)(v16 - 8);
          v239 = *(_QWORD *)(v16 - 16);
          v938 = v16 - 8;
          v7 = *(sem_t **)(v16 - 16);
          LODWORD(v240) = PyNumber_InPlaceXor(v7, *(_QWORD *)(v16 - 8));
LABEL_394:
          v637 = (*(_QWORD *)v239)-- == 1LL;
          v15 = v240;
          v16 = v938;
          if ( v637 )
          {
            v7 = (sem_t *)v239;
            (*(void (__fastcall **)(__int64))(*(_QWORD *)(v239 + 8) + 48LL))(v239);
            v16 = v938;
          }
          goto LABEL_320;
      }
    }
  }
  v65 = *(_QWORD *)(PyFrameObject + 64);
LABEL_70:
  if ( (unsigned int)(((signed __int64)v7 - v65) >> 3) )
  {
    v493 = (signed __int64)(&v7[-1].__align + 3);
    v494 = *(&v7[-1].__align + 3);
    if ( v494 )
    {
      v637 = (*(_QWORD *)v494)-- == 1LL;
      if ( v637 )
      {
        (*(void (**)(void))(*(_QWORD *)(v494 + 8) + 48LL))();
        v65 = *(_QWORD *)(PyFrameObject + 64);
      }
      else
      {
        v65 = *(_QWORD *)(PyFrameObject + 64);
      }
    }
    if ( (unsigned int)((v493 - v65) >> 3) )
    {
      v495 = *(_QWORD *)(v493 - 8);
      if ( v495 )
      {
        v637 = (*(_QWORD *)v495)-- == 1LL;
        if ( v637 )
        {
          (*(void (**)(void))(*(_QWORD *)(v495 + 8) + 48LL))();
          v65 = *(_QWORD *)(PyFrameObject + 64);
        }
        else
        {
          v65 = *(_QWORD *)(PyFrameObject + 64);
        }
      }
      if ( (unsigned int)((v493 - 8 - v65) >> 3) )
      {
        v496 = (__int64 *)(v493 - 16);
        v497 = *v496;
        if ( *v496 )
          goto LABEL_829;
        while ( 1 )
        {
          v10 = ((signed __int64)v496 - v65) >> 3;
          if ( !(_DWORD)v10 )
            break;
          --v496;
          v497 = *v496;
          if ( *v496 )
          {
LABEL_829:
            v637 = (*(_QWORD *)v497)-- == 1LL;
            if ( v637 )
              (*(void (**)(void))(*(_QWORD *)(v497 + 8) + 48LL))();
            v65 = *(_QWORD *)(PyFrameObject + 64);
          }
        }
      }
    }
  }
  v66 = 0LL;
  if ( v63 == 8 )
    v66 = v1010;
LABEL_73:
  if ( *((_DWORD *)v920 + 8) )
  {
    v677 = (int (__fastcall *)(__int64, signed __int64, signed __int64, __int64, signed __int64, signed __int64))*((_QWORD *)v920 + 6);
    if ( !v677 )
      goto LABEL_1234;
    if ( v63 == 8 || v63 == 64 )
    {
      v678 = *(_QWORD *)(PyFrameObject + 112);
      v679 = *((_QWORD *)v920 + 8);
      if ( *(_DWORD *)(v678 + 28)
        || ((*(_DWORD *)(v678 + 28) = 1,
             *(_DWORD *)(v678 + 32) = 0,
             LOBYTE(v10) = v63 == 8 || v63 == 64,
             v680 = v677(v679, PyFrameObject, 3LL, v66, v9, v10),
             !*(_QWORD *)(v678 + 48)) ? (v681 = *(_QWORD *)(v678 + 40) != 0LL) : (v681 = 1),
            --*(_DWORD *)(v678 + 28),
            *(_DWORD *)(v678 + 32) = v681,
            !v680) )
      {
LABEL_1234:
        v682 = *((_QWORD *)v920 + 5);
        if ( !v682 )
          goto LABEL_74;
        if ( v63 != 2 )
          goto LABEL_1236;
LABEL_1245:
        sub_563E80(v682, *((_QWORD *)v920 + 7), PyFrameObject, 3LL, 0LL);
        goto LABEL_74;
      }
      if ( v66 && (v637 = *(_QWORD *)v66 == 1LL, --*(_QWORD *)v66, v637) )
      {
        v785 = *(_QWORD *)(v66 + 8);
        v786 = v66;
        v66 = 0LL;
        (*(void (__fastcall **)(__int64))(v785 + 48))(v786);
      }
      else
      {
        v66 = 0LL;
      }
    }
    else
    {
      if ( v63 != 2 )
      {
        v682 = *((_QWORD *)v920 + 5);
        if ( !v682 )
          goto LABEL_74;
LABEL_1236:
        if ( sub_5B63D0(v682, *((_QWORD *)v920 + 7), PyFrameObject, 3LL, v66) )
        {
          if ( v66 )
          {
            v637 = (*(_QWORD *)v66)-- == 1LL;
            if ( v637 )
              (*(void (__fastcall **)(__int64))(*(_QWORD *)(v66 + 8) + 48LL))(v66);
          }
          v66 = 0LL;
        }
        goto LABEL_74;
      }
      sub_563E80(*((_QWORD *)v920 + 6), *((_QWORD *)v920 + 8), PyFrameObject, 3LL, 0LL);
    }
    v682 = *((_QWORD *)v920 + 5);
    if ( !v682 )
      goto LABEL_74;
    goto LABEL_1245;
  }
LABEL_74:
  v67 = *((_QWORD *)v920 + 2);
  v68 = *(_QWORD **)(v67 + 88);
  if ( v68 )
  {
    v435 = *((_QWORD *)v920 + 12);
    v436 = *((_QWORD *)v920 + 13);
    v437 = *((_QWORD *)v920 + 14);
    ++*v68;
    v438 = *(_QWORD **)(v67 + 96);
    if ( v438 )
      ++*v438;
    v439 = *(_QWORD **)(v67 + 104);
    if ( v439 )
      ++*v439;
    *((_QWORD *)v920 + 12) = *(_QWORD *)(v67 + 88);
    *((_QWORD *)v920 + 13) = *(_QWORD *)(v67 + 96);
    *((_QWORD *)v920 + 14) = *(_QWORD *)(v67 + 104);
    if ( v435 )
    {
      v637 = (*(_QWORD *)v435)-- == 1LL;
      if ( v637 )
        (*(void (**)(void))(*(_QWORD *)(v435 + 8) + 48LL))();
    }
    if ( v436 )
    {
      v637 = (*(_QWORD *)v436)-- == 1LL;
      if ( v637 )
        (*(void (__fastcall **)(__int64))(*(_QWORD *)(v436 + 8) + 48LL))(v436);
    }
    if ( v437 )
    {
      v637 = (*(_QWORD *)v437)-- == 1LL;
      if ( v637 )
        (*(void (__fastcall **)(__int64))(*(_QWORD *)(v437 + 8) + 48LL))(v437);
    }
    v440 = *(_QWORD *)(v67 + 88);
    PySys_SetObject("exc_type");
    v441 = *(_QWORD *)(v67 + 96);
    PySys_SetObject(&unk_61725B);
    v442 = *(_QWORD *)(v67 + 104);
    PySys_SetObject("exc_traceback");
    v443 = *(_QWORD *)(v67 + 88);
    v444 = *(_QWORD *)(v67 + 96);
    v445 = *(_QWORD *)(v67 + 104);
    *(_QWORD *)(v67 + 88) = 0LL;
    *(_QWORD *)(v67 + 96) = 0LL;
    *(_QWORD *)(v67 + 104) = 0LL;
    v637 = (*(_QWORD *)v443)-- == 1LL;
    if ( v637 )
      (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v443 + 8) + 48LL))(v443, v442);
    if ( v444 )
    {
      v637 = (*(_QWORD *)v444)-- == 1LL;
      if ( v637 )
        (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v444 + 8) + 48LL))(v444, v442);
    }
    if ( v445 )
    {
      v637 = (*(_QWORD *)v445)-- == 1LL;
      if ( v637 )
        (*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)(v445 + 8) + 48LL))(v445, v442);
    }
  }
LABEL_75:
  --*((_DWORD *)PyThreadState_Current + 6);
  *((_QWORD *)v920 + 2) = *(_QWORD *)(PyFrameObject + 24);
  return v66;
}