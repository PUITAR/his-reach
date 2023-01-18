include ../../make_config.mk

ifndef DISABLE_JEMALLOC
	ifdef JEMALLOC
		PLATFORM_CXXFLAGS += -DROCKSDB_JEMALLOC -DJEMALLOC_NO_DEMANGLE
	endif
	EXEC_LDFLAGS := $(JEMALLOC_LIB) $(EXEC_LDFLAGS) -lpthread
	PLATFORM_CXXFLAGS += $(JEMALLOC_INCLUDE)
endif

ifneq ($(USE_RTTI), 1)
	CXXFLAGS += -fno-rtti
endif

CFLAGS += -Wstrict-prototypes

.PHONY: clean librocksdb

all: TILL_Construct  TILL_Reach test

order: librocksdb order.cc
	@$(CXX) $(CXXFLAGS) $@.cc -o$@ ../../librocksdb.a -I../../include -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

clear: 
	@rm -rf ../db/*
	@rm -rf index/*

TILL_Construct: librocksdb TILL_Construct.cc
	@$(CXX) $(CXXFLAGS) $@.cc -g -o$@ ../../librocksdb.a -I../../include -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)


test: librocksdb test.cc
	@$(CXX) $(CXXFLAGS) $@.cc -o$@ ../../librocksdb.a -I../../include -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

TILL_Reach: librocksdb TILL_Reach.cc
	@$(CXX) $(CXXFLAGS) $@.cc -o$@ ../../librocksdb.a -I../../include -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

testcase:
	@$(CXX) $(CXXFLAGS) $@.cc -o$@ ../../librocksdb.a -I../../include -O2 -std=c++17 $(PLATFORM_LDFLAGS) $(PLATFORM_CXXFLAGS) $(EXEC_LDFLAGS)

# tiny.txt
dp = ../tiny.txt
cond = 1 12 1 9
# CollegeMsg
dp = ../CollegeMsg.txt
cond = 1 1899 1 16736182
# a.txt
dp = ../a.txt
cond = 1 400 1 16727039
cond = 1 500 1 16727039

# CMDs
mkdb:
	@../online_reach/mkdb $(dp)
ord:
	@./order $(cond)

diff:
	@./testcase $(cond) 100000
	@./TILL_Reach $(cond) testcase.txt till_res.txt
	@../online_reach/Online_Reach $(cond) testcase.txt online_res.txt
	@diff online_res.txt till_res.txt

till:
	@./TILL_Construct $(cond)

tes:
	@./test $(cond)
