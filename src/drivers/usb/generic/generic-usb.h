/*
 * generic-usb.c
 *
 * Generic USB module for libusb.
 */

/*
 * Copyright (C) 2005 Adam Kropelin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General
 * Public License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef _GENERICUSB_H
#define _GENERICUSB_H

#include "../usb.h"
#include "libusb.h"
#include "HidUps.h"

class GenericUsbUpsDriver: public UsbUpsDriver
{
public:
   GenericUsbUpsDriver(UPSINFO *ups);
   virtual ~GenericUsbUpsDriver() {}

   // Inherited from UpsDriver
   virtual bool Open();
   virtual bool Close();
   virtual bool check_state();

   // Inherited from UsbUpsDriver
   virtual int write_int_to_ups(int ci, int value, char const* name);
   virtual int read_int_from_ups(int ci, int *value);

protected:

   // Inherited from UsbUpsDriver
   virtual bool pusb_ups_get_capabilities();
   virtual bool pusb_get_value(int ci, USB_VALUE *uval);

private:

   /*
    * When we are traversing the USB reports given by the UPS and we
    * find an entry corresponding to an entry in the known_info table
    * above, we make the following USB_INFO entry in the info table
    * of our private data.
    */
   typedef struct s_usb_info {
      unsigned usage_code;            /* usage code wanted */
      unsigned unit_exponent;         /* exponent */
      unsigned unit;                  /* units */
      int data_type;                  /* data type */
      hid_item_t item;                /* HID item (for read) */
      hid_item_t witem;               /* HID item (for write) */
      int report_len;                 /* Length of containing report */
      int ci;                         /* which CI does this usage represent? */
      int value;                      /* Previous value of this item */
   } USB_INFO;

   void reinitialize_private_structure();
   bool open_usb_device();
   bool usb_link_check();
   bool populate_uval(USB_INFO *info, unsigned char *data, USB_VALUE *uval);

   USB_INFO *_info[CI_MAXCI + 1];   /* Info pointers for each command */
   bool _linkcheck;
   HidUps _hidups;
};

#endif
