CLAGS += -std=c11
CFLAFS += -Wall -Wextra -Wpedantic -Wwrite-strings -Winline
CFLAGS += -Wstack-usage=1024 -Wfloat-equal -Waggregate-return 
LDLIBS += -lssl -lcrypto
RISK_BIN=risk
RISK_DEPS=risk.o 

all:$(RISK_BIN)

$(RISK_BIN):$(RISK_DEPS)

.PHONY: clean debug profile

debug: CFLAGS+=-g
debug: all

profile: CFLAGS+=-pg
profile: all

clean:
	$(RM) $(RISK_BIN) $(RISK_DEPS)

