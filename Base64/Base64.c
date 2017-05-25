#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int Base64Encode(unsigned char * in, unsigned char * out);
int Base64Decode (unsigned char * in, unsigned char * out);

int main (int argc, char *argv[])
{
	unsigned char out[1024];
	if (argc < 3)
	{
		fprintf (stderr, "usage: %s\n", strerror (errno));
		exit (1);
	}
	if (atoi (argv[1]) == 0)
	{
		Base64Encode (argv[2], out);
		fprintf (stdout, "%s", out);
	}
	else if (atoi (argv[1]) == 1)
	{
		Base64Decode (argv[2], out); 
		fprintf (stdout, "%s", out);
	}
	return 0;
}

unsigned char EncodeChar (unsigned char c_in)
{
	unsigned char c_out;
	if (c_in <= 25)
	{
		c_out = c_in + 65;
	}
	else if (c_in <= 51)
	{
		c_out = c_in + 71;
	}
	else if (c_in < 61)
	{
		c_out = c_in - 4;
	}
	else if (c_in == 62)
	{
		c_out = '+';
	}
	else
	{
		c_out = '/';
	}
	return c_out;
}

unsigned char DecodeChar (unsigned char c_in)
{
	unsigned char c_out;
	if (c_in == '+')
	{
		c_out = 62;
	}
	else if (c_in == '/')
	{
		c_out = 63;
	}
	else if (c_in <= 57)
	{
		c_out = c_in + 4;
	}
	else if (c_in <= 90)
	{
		c_out = c_in - 65;
	}
	else
	{
		c_out = c_in - 71;
	}
	
	return c_out;
}

int Base64Encode(unsigned char * in, unsigned char * out)
{
	unsigned char buf[1024] = {0}, pk[4] = {0}; 
	int len = 0, z = 0, i, j;
	len = strlen (in);
	
	strncpy (buf, in, len);
	z = len%3;
	if (z == 2)
	{
		len ++;
	}
	else if (z == 1)
	{
		len = len + 2;
	}
	else 
	{
		;
	}
	
	len = 0;
	for (i=0; i<strlen (buf); )
	{
		pk[0] = buf[i] >> 2;
		pk[1] = ((buf[i] << 4) & 0x30) | (buf[i+1] >> 4);  
		pk[2] = ((buf[i+1] << 2) & 0x3C) | (buf[i+2] >> 6);
		pk[3] = buf[i+2] & 0x3F;
		i += 3;
		for (j=0; j<4; j++)
		{
			out[len++] = EncodeChar (pk[j]);
		}
	}
	if (z == 2)
	{
		out[len-1] = '=';
	}
	else if (z == 1)
	{
		out[len-1] = '=';
		out[len-2] = '=';
	}
	out[len] = 0;
	return 0;
}

int Base64Decode (unsigned char * in, unsigned char * out)
{
	unsigned char buf[1024] = {0}, pk[3] = {0};
	int len = 0, i, j, k = 0; 
	
	len = strlen (in);
	strncpy (buf, in, len); 
	while (buf[len-1] == '=')
	{
		len --;
	}
	buf[len] = 0;
	for (i=0; i<len; i++)
	{
		buf[i] = DecodeChar (in[i]);
	}
	for (i=0; i<len; )
	{
		pk[0] = (buf[i] << 2) | ((buf[i+1] >> 4) & 0x03);
		pk[1] = (buf[i+1] << 4) | ((buf[i+2] & 0x3C) >> 2);
		pk[2] = (buf[i+2] << 6) | buf[i+3];
		i += 4;
		for (j=0; j<3; j++)
		{
			out[k++] = pk[j];
		}
	}
	out[k] = 0;
	return 0;
}
