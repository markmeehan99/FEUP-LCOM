PROG=proj

SRCS = proj.c chicken.c game.c video_gr.c timer.c keyboard.c bitmap.c obstacles.c mouse.c crosshair.c gameState.c rtc.c menu.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
