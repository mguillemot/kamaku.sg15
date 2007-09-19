#include "sysace_stdio.h"
#include "xparameters.h"
#include "xstatus.h"

/*
 * bootloader
 *
 * @author: Matthieu Guillemot <g@npng.org>
 * @date: 03/06/2005
 * @version: 1.00
 *
 *
 * Ce programme est destin� � �tre charg� dans la BRAM de la console
 * lors de la programmation du FPGA. Il lit sur la carte Compact Flash
 * un programme au format Motorola S-record (par d�faut "boot.me" � la
 * racine de la carte, peut �tre modifi� dans le main() de ce fichier),
 * le v�rifie, le charge en m�moire, puis le boote.
 *
 *
 * !! SCRIPTS DE LIAISON !!
 * Les conventions suivantes doivent �tre respect�es entre le bootloader
 * et le programme � charger :
 * - le bootloader doit r�server le d�but de la BRAM pour les sections
 *   du programme � charger qui ne seront pas copi�es dans la m�moire
 *   principale : .boot0 .boot
 *   Il est conseill� de r�server les 0x1000 premiers octets de la BRAM
 *   � ces fins, ce qui est le r�le de la section .boot0_fils du script
 *   de chargement du bootloader
 * - le programme � charger ne doit pas utiliser trop de donn�es en BRAM.
 *   Typiquement, seules les sections .boot0 et .boot y seront stock�es, 
 *   et ne devront pas exc�der 0x1000 octets.
 *
 *
 * Structure des fichiers S-record :
 *   http://www.amelek.gda.pl/avr/uisp/srecord.htm
 *
 * Convertir un ELF en S-record :
 *   powerpc-eabi-objcopy -O srec source.elf destination.srec
 *   (sous la console de l'EDK)
 */

#define DISPLAY_PROGRESS 1

/*** Types d�finis ***/

typedef enum { S0, S1, S2, S3, S5, S7, S8, S9 } SType;

typedef struct
{
  SType   type;
  Xuint32 address;
  Xint8   data_size;
  Xuint8  data[32];
  Xuint32 computed_checksum;
  Xuint8  read_checksum;
} SRecord;


/*** Fonctions de conversion ***/

Xuint32 hex_value(Xuint8 a)
{
  if (a >= '0' && a <= '9')
  {
    return (a - '0');
  }
  else if (a >= 'A' && a <= 'F')
  {
    return (a - 'A') + 10;
  }
  return 0;
}

Xuint32 hex2_to_int(Xuint8 *a)
{
  return (hex_value(a[0]) * 16 + hex_value(a[1]));
}

Xuint32 hex4_to_int(Xuint8 *a)
{
  return (hex2_to_int(a) * 256 + hex2_to_int(a + 2));
}

Xuint32 hex6_to_int(Xuint8 *a)
{
  return (hex2_to_int(a) * 256 * 256 + hex4_to_int(a + 2));
}

Xuint32 hex8_to_int(Xuint8 *a)
{
  return (hex4_to_int(a) * 256 * 256 + hex4_to_int(a + 4));
}


/*** Fonctions principales ***/

XStatus read_SRecord(SYSACE_FILE *filename, SRecord *sr)
{
  /*
   * Selon http://www.amelek.gda.pl/avr/uisp/srecord.htm, il faudrait pr�voir
   * un buffer de 255*2 caract�res. powerpc-eabi-objcopy semble respecter la
   * norme Motorola et ne jamais utiliser plus de 64 caract�res de donn�es par
   * S-record. Et une petite �conomie de faire sur la taille du bootloader !
   */
  Xuint8 readBuffer[64];
  Xint32 numread, count, i;

  // initialisation du checksum calcul�
  sr->computed_checksum = 0;

  // lecture du champ "type"
  numread = sysace_fread(readBuffer, 1, 2, filename);
  if (numread != 2)
  {
    return XST_FAILURE;
  }
  switch (readBuffer[1])
  {
    case '0':
      sr->type = S0;
      break;
    case '1':
      sr->type = S1;
      break;
    case '2':
      sr->type = S2;
      break;
    case '3':
      sr->type = S3;
      break;
    case '5':
      sr->type = S5;
      break;
    case '7':
      sr->type = S7;
      break;
    case '8':
      sr->type = S8;
      break;
    case '9':
      sr->type = S9;
      break;
    default:
      return 0;
  }

  // lecture du champ "count"
  numread = sysace_fread(readBuffer, 1, 2, filename);
  if (numread != 2)
  {
    return XST_FAILURE;
  }
  count = hex2_to_int(readBuffer);
  sr->computed_checksum += count;
  sr->data_size = count - 1;

  // lecture du champ "address"
  if (sr->type == S0 || sr->type == S1 || sr->type == S5 || sr->type == S9)
  {
    numread = sysace_fread(readBuffer, 1, 4, filename);
    if (numread != 4)
    {
      return XST_FAILURE;
    }
    sr->data_size -= 2;
    sr->address = hex4_to_int(readBuffer);
    sr->computed_checksum += hex2_to_int(readBuffer);
    sr->computed_checksum += hex2_to_int(readBuffer + 2);
  }
  else if (sr->type == S2 || sr->type == S8)
  {
    numread = sysace_fread(readBuffer, 1, 6, filename);
    if (numread != 6)
    {
      return XST_FAILURE;
    }
    sr->data_size -= 3;
    sr->address = hex6_to_int(readBuffer);
    sr->computed_checksum += hex2_to_int(readBuffer);
    sr->computed_checksum += hex2_to_int(readBuffer + 2);
    sr->computed_checksum += hex2_to_int(readBuffer + 4);
  }
  else // S3, S7
  {
    numread = sysace_fread(readBuffer, 1, 8, filename);
    if (numread != 8)
    {
      return XST_FAILURE;
    }
    sr->data_size -= 4;
    sr->address = hex8_to_int(readBuffer);
    sr->computed_checksum += hex2_to_int(readBuffer);
    sr->computed_checksum += hex2_to_int(readBuffer + 2);
    sr->computed_checksum += hex2_to_int(readBuffer + 4);
    sr->computed_checksum += hex2_to_int(readBuffer + 6);
  }

  // lecture du champ "data"
  numread = sysace_fread(readBuffer, 1, 2 * sr->data_size, filename);
  if (numread != 2 * sr->data_size)
  {
    return XST_FAILURE;
  }
  for (i = 0; i < sr->data_size; i++)
  {
    sr->data[i] = (Xuint8)(hex2_to_int(readBuffer + 2 * i));
    sr->computed_checksum += sr->data[i];
  }

  // lecture du champ "cheksum"
  numread = sysace_fread(readBuffer, 1, 2, filename);
  if (numread != 2)
  {
    return XST_FAILURE;
  }
  sr->read_checksum = (Xuint8)(hex2_to_int(readBuffer));

  // lecture du retour charriot
  numread = sysace_fread(readBuffer, 1, 2, filename);
  if (numread != 2)
  {
    return XST_FAILURE;
  }

  // OK desu !
  return XST_SUCCESS;
}

XStatus check_SRecord(SRecord *sr)
{
  // V�rification du checksum...
  Xuint8 cs = 0;
  cs = (Xuint8)(sr->computed_checksum & 0xff);
  cs = ~cs;
  return (sr->read_checksum == cs) ? XST_SUCCESS : XST_FAILURE;
}

XStatus load_SRecord(SRecord *sr, Xuint32 *start_address)
{
  Xint32 i;
  Xuint8 *ptr;
  if (sr->type == S0)
  {
    xil_printf("Information data read from srec file :\r\n  module name = ", sr->data);
    for (i = 0; i < 10; i++)
    {
      xil_printf("%c", sr->data[i]);
    }
    print("\r\n  description = ");
    for (i = 10; i < sr->data_size; i++)
    {
      xil_printf("%c", sr->data[i]);
    }
    print("\r\n");
  }
  else if (sr->type == S5)
  {
    // Ca ne devrait jamais arriver, donc on ne fait rien !
  }
  else if (sr->type == S7 || sr->type == S8 || sr->type == S9)
  {
    // Adresse de la premi�re instruction
    *start_address = sr->address;
  }
  else
  {
    // Chargement de la m�moire
    ptr = (Xuint8 *)sr->address;
    for (i = 0; i < sr->data_size; i++)
    {
      *ptr = sr->data[i];
      ptr++;
    }
  }
  return XST_SUCCESS;
}

XStatus load_srec(char *filename, Xuint32 *start_address)
{
  SYSACE_FILE *infile;
  SRecord      sr;
  XStatus      status;
  Xint32       cpt;

  cpt = 1;
  infile = sysace_fopen(filename, "r");
  if (infile)
  {
    xil_printf("Loading S-record file %s...\r\n", filename);
    status = read_SRecord(infile, &sr);
    while (status == XST_SUCCESS)
    {
      status = check_SRecord(&sr);
      if (status != XST_SUCCESS)
      {
        xil_printf("ERROR: checksum error at line %d", cpt);
        return;
      }
      status = load_SRecord(&sr, start_address);
      if (status != XST_SUCCESS)
      {
        xil_printf("ERROR: memory error at line %d", cpt);
        return;
      }
      cpt++;
      status = read_SRecord(infile, &sr);
    }
    sysace_fclose(infile);
    xil_printf("Loading S-record file %s... done !\r\n", filename);
    xil_printf("Program start address is 0x%08X\r\n", *start_address);
    return XST_SUCCESS;
  }
  else
  {
    xil_printf("Unable to open file %s.\r\n", filename);
  return XST_FAILURE;
  }
}

int main()
{
  /*
   * L'adresse de d�marrage d�cod�e depuis le fichier S-record n'est pas
   * prise en compte, puisque le processeur PowerPC ne peut d�marrer qu'�
   * l'adresse 0xFFFFFFFC. Une fois le fichier S-record charg� en m�moire,
   * on demande au processeur de se reseter pour booter le nouveau programme.
   */
  Xuint32 start_address;
  XStatus status;

  XCache_DisableDCache();
  XCache_DisableICache();
  print("Starting bootloader.\r\n");
  status = load_srec("a:\\boot.me", &start_address);
  if (status == XST_SUCCESS)
  {
    // branchement
    print("All right ! Let's go, girls !\r\n\r\n");
    usleep(100000);
    asm("lis 0,0x3000");
    asm("mtdbcr0 0"); // soft reset powaaa !!
  }
  print("Boot failed.\r\n");
  return 0;
}
