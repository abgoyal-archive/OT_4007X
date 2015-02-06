

#define NSEMI 0
#define NCMD 1
#define NPIPE 2
#define NREDIR 3
#define NBACKGND 4
#define NSUBSHELL 5
#define NAND 6
#define NOR 7
#define NIF 8
#define NWHILE 9
#define NUNTIL 10
#define NFOR 11
#define NCASE 12
#define NCLIST 13
#define NDEFUN 14
#define NARG 15
#define NTO 16
#define NCLOBBER 17
#define NFROM 18
#define NFROMTO 19
#define NAPPEND 20
#define NTOFD 21
#define NFROMFD 22
#define NHERE 23
#define NXHERE 24
#define NNOT 25



struct nbinary {
      int type;
      union node *ch1;
      union node *ch2;
};


struct ncmd {
      int type;
      int backgnd;
      union node *args;
      union node *redirect;
};


struct npipe {
      int type;
      int backgnd;
      struct nodelist *cmdlist;
};


struct nredir {
      int type;
      union node *n;
      union node *redirect;
};


struct nif {
      int type;
      union node *test;
      union node *ifpart;
      union node *elsepart;
};


struct nfor {
      int type;
      union node *args;
      union node *body;
      char *var;
};


struct ncase {
      int type;
      union node *expr;
      union node *cases;
};


struct nclist {
      int type;
      union node *next;
      union node *pattern;
      union node *body;
};


struct narg {
      int type;
      union node *next;
      char *text;
      struct nodelist *backquote;
};


struct nfile {
      int type;
      union node *next;
      int fd;
      union node *fname;
      char *expfname;
};


struct ndup {
      int type;
      union node *next;
      int fd;
      int dupfd;
      union node *vname;
};


struct nhere {
      int type;
      union node *next;
      int fd;
      union node *doc;
};


struct nnot {
      int type;
      union node *com;
};


union node {
      int type;
      struct nbinary nbinary;
      struct ncmd ncmd;
      struct npipe npipe;
      struct nredir nredir;
      struct nif nif;
      struct nfor nfor;
      struct ncase ncase;
      struct nclist nclist;
      struct narg narg;
      struct nfile nfile;
      struct ndup ndup;
      struct nhere nhere;
      struct nnot nnot;
};


struct nodelist {
	struct nodelist *next;
	union node *n;
};


union node *copyfunc(union node *);
void freefunc(union node *);
