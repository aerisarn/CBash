/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is CBash code.
 *
 * The Initial Developer of the Original Code is
 * Waruddar.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#pragma once
#include "../../Common.h"
#include "../../GenericRecord.h"

namespace FNV
{
class ACHRRecord : public FNVRecord //Placed NPC
    {
    private:
        enum xespFlags
            {
            fIsOppositeParent = 0x00000001,
            fIsPopIn          = 0x00000002
            };
    public:
        StringRecord EDID; //Editor ID
        ReqSimpleSubRecord<FORMID> NAME; //Base
        OptSimpleSubRecord<FORMID> XEZN; //Encounter Zone
        RawRecord XRGD; //Ragdoll Data
        RawRecord XRGB; //Ragdoll Biped Data
        OptSubRecord<GENPATROL> Patrol; //Patrol Data
        OptSimpleSubRecord<int32_t> XLCM; //Level Modifier

        OptSimpleSubRecord<FORMID> XMRC; //Merchant Container
        OptSimpleSubRecord<int32_t> XCNT; //Count
        OptSimpleFloatSubRecord<flt_0> XRDS; //Radius
        OptSimpleFloatSubRecord<flt_0> XHLP; //Health
        UnorderedSparseArray<GENXDCR *> XDCR; //Linked Decals
        OptSimpleSubRecord<FORMID> XLKR; //Linked Reference
        OptSubRecord<GENXCLP> XCLP; //Linked Reference Color
        OptSubRecord<GENACTPARENT> ActivateParents; //Activate Parents
        StringRecord XATO; //Activation Prompt
        OptSubRecord<GENXESP> XESP; //Enable Parent
        OptSimpleSubRecord<FORMID> XEMI; //Emittance
        OptSimpleSubRecord<FORMID> XMBR; //MultiBound Reference
        OptSimpleSubRecord<uint8_t> XIBS; //Ignored By Sandbox (Empty, boolean, if != 0, write empty)
        OptSimpleFloatSubRecord<flt_1> XSCL; //Scale
        ReqSubRecord<GENPOSDATA> DATA; //Position/Rotation

        ACHRRecord(unsigned char *_recData=NULL);
        ACHRRecord(ACHRRecord *srcRecord);
        ~ACHRRecord();

        bool   VisitFormIDs(FormIDOp &op);

        bool   IsOppositeParent();
        void   IsOppositeParent(bool value);
        bool   IsPopIn();
        void   IsPopIn(bool value);
        bool   IsFlagMask(uint8_t Mask, bool Exact=false);
        void   SetFlagMask(uint8_t Mask);

        uint32_t GetFieldAttribute(DEFAULTED_FIELD_IDENTIFIERS, uint32_t WhichAttribute=0);
        void * GetField(DEFAULTED_FIELD_IDENTIFIERS, void **FieldValues=NULL);
        bool   SetField(DEFAULTED_FIELD_IDENTIFIERS, void *FieldValue=NULL, uint32_t ArraySize=0);
        void   DeleteField(DEFAULTED_FIELD_IDENTIFIERS);

        uint32_t GetType();
        char * GetStrType();

        int32_t ParseRecord(unsigned char *buffer, unsigned char *end_buffer, bool CompressedOnDisk=false);
        int32_t Unload();
        int32_t WriteRecord(FileWriter &writer);

        bool operator ==(const ACHRRecord &other) const;
        bool operator !=(const ACHRRecord &other) const;
        bool equals(Record *other);
        bool deep_equals(Record *master, RecordOp &read_self, RecordOp &read_master, std::unordered_set<Record *> &identical_records);
    };
}