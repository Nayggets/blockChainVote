CC=gcc
CFLAGS=`pkg-config --cflags glib-2.0 sqlite3` -lssl -lcrypto
LIBS=`pkg-config --libs glib-2.0 sqlite3` -lsqlite3  -pthread -lgmp -g -lcrypto -lssl 
BUILDDIR=../build
PROJECTLIBDIR=-L../build/lib
PROJECTLIB=-lvoting


$(shell mkdir -p $(BUILDDIR))

all: $(BUILDDIR)/test_electeur $(BUILDDIR)/test_election $(BUILDDIR)/testsqlite $(BUILDDIR)/test_creation_electeur $(BUILDDIR)/server_test $(BUILDDIR)/test_crypto.bin $(BUILDDIR)/test_vote

$(BUILDDIR)/test_electeur: test_electeur.c
	$(CC) test_electeur.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_electeur

$(BUILDDIR)/test_election: test_election.c
	$(CC) test_election.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_election

$(BUILDDIR)/test_vote: test_vote.c
	$(CC) test_vote.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_vote


$(BUILDDIR)/testsqlite: testsqlite.c
	$(CC) testsqlite.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/testsqlite

$(BUILDDIR)/server_test: testsqlite.c
	$(CC) ./server_test.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/serveur

$(BUILDDIR)/test_creation_electeur: test_creation_electeur.c
	$(CC) test_creation_electeur.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_creation_electeur

$(BUILDDIR)/test_crypto.bin: test_crypto.c
	$(CC) test_crypto.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -lgmp -o $(BUILDDIR)/test_crypto.bin

clean:
	rm $(BUILDDIR)/test_vote; rm $(BUILDDIR)/test_election; rm $(BUILDDIR)/test_electeur 
