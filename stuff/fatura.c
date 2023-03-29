#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define INVOICESTART 12

void edit_address(char *address){
  int i, j;
  char tmp[1000];
  j=0;
  for (i=0;i<strlen(address);i++){
    if (address[i]!=';' && address[i]!='/' && address[i]!='|' && address[i]!='&')
      tmp[j++]=address[i];
    else if (address[i]=='&'){
      tmp[j++]='\\';
      tmp[j++]='&';
    }
    else{
      tmp[j++]='\\';
      tmp[j++]='\\';
    }
  }
  tmp[j]=0;
  strcpy(address, tmp);
}

int main(int argc, char **argv){
  FILE *in=fopen(argv[1], "r");
  FILE *out;
  char *token;
  char line[10000];
  char name[100], lname[100], address[1000], paydate[100], credit[100];
  int amount;
  float vat;
  float subtotal;
  int invno = INVOICESTART;
  int skip;
  if (in==NULL) return -1;
  fgets(line, 10000, in); // skip header
  while(1){
    if (feof(in)) break;
    fgets(line, 10000, in);
    if (feof(in)) break;
    token = strtok (line, "\t"); //sira
    token = strtok (NULL, "\t"); //name
    strcpy(name, token);
    token = strtok (NULL, "\t"); //lname
    strcpy(lname, token);
    token = strtok (NULL, "\t"); //paydate
    strcpy(paydate, token);
    token = strtok (NULL, "\t"); //address
    strcpy(address, token);
    token = strtok (NULL, "\t"); //expl.
    token = strtok (NULL, "\t"); //credit
    strcpy(credit, token);
    //token = strtok (NULL, "\t"); //kdv
    token = strtok (NULL, "\t"); //amount
    skip=0;
    while (!isdigit(token[skip])) skip++;
    amount = atoi(token+skip);
    edit_address(address);
    //printf ("%s - %s - %s - %s - %d- \n", name, lname, paydate, address, amount);

    sprintf(line, "header.tex");
    out = fopen(line, "w");

    subtotal = amount / 1.18;
    vat = amount - subtotal;
    fprintf(out, "\\newcommand{\\registrant}{%s %s}\n", name, lname);
    fprintf(out, "\\newcommand{\\invoice}{%d}\n", invno);
    fprintf(out, "\\newcommand{\\invaddress}{%s}\n", address);
    fprintf(out, "\\newcommand{\\invdate}{%s} \n", paydate);
    fprintf(out, "\\newcommand{\\subtotal}{%.2f}\n", subtotal);
    fprintf(out, "\\newcommand{\\vat}{%.2f}\n", vat);
    fprintf(out, "\\newcommand{\\fee}{%d} \n", amount);
    fprintf(out, "\\newcommand{\\creditcard}{%s} \n", credit);
    fprintf(out, " \n");

    fclose(out);

    system("pdflatex invoice.tex");
    system("pdflatex invoice.tex");

    sprintf(line, "mv invoice.pdf invoice-%s_%s-%d.pdf", name, lname, invno);
    system(line);

    invno++;
  }
  return 0;
}
