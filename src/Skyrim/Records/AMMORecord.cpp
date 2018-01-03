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
#include "../../Common.h"
#include "../../ModFile.h"
#include "AMMORecord.h"

namespace Sk
{
	AMMORecord::AMMORecord(unsigned char *_recData) :
		TES5Record(_recData)
	{
		//
	}

	AMMORecord::AMMORecord(AMMORecord *srcRecord) :
		TES5Record()
	{
		if (srcRecord == NULL)
			return;

		flags = srcRecord->flags;
		formID = srcRecord->formID;
		flagsUnk = srcRecord->flagsUnk;
		formVersion = srcRecord->formVersion;
		versionControl2[0] = srcRecord->versionControl2[0];
		versionControl2[1] = srcRecord->versionControl2[1];

		recData = srcRecord->recData;
		if (!srcRecord->IsChanged())
			return;

		EDID = srcRecord->EDID;
		OBND = srcRecord->OBND;
		FULL = srcRecord->FULL;
		MODL = srcRecord->MODL;
		ICON = srcRecord->ICON;
		MICO = srcRecord->MICO;
		DEST = srcRecord->DEST;
		YNAM = srcRecord->YNAM;
		ZNAM = srcRecord->ZNAM;
		DESC = srcRecord->DESC;
		KWDA = srcRecord->KWDA;
		DATA = srcRecord->DATA;

		if (!srcRecord->IsChanged())
			return;

		return;
	}

	AMMORecord::~AMMORecord()
	{
		//
	}

	uint32_t AMMORecord::GetType()
	{
		return REV32(AMMO);
	}

	char * AMMORecord::GetStrType()
	{
		return "AMMO";
	}


	bool AMMORecord::VisitFormIDs(FormIDOp &op)
	{
		if (!IsLoaded())
			return false;
		//TODO - implement VisitFormIDs for struct GENOBND

		if (DEST.IsLoaded()) {
			//TODO - implement VisitFormIDs for struct GENDESTRUCT
		}
		if (YNAM.IsLoaded()) {
			op.Accept(YNAM.value);
		}
		if (ZNAM.IsLoaded()) {
			op.Accept(ZNAM.value);
		}
		if (KWDA.IsLoaded()) {
			for (uint32_t i = 0; i < KWDA.value.size(); ++i) {

				op.Accept(KWDA.value[i]);

			};
		}
		if (DATA.IsLoaded()) {
			//TODO - implement VisitFormIDs for struct AMMODATA
		}

		return op.Stop();
	}


	int32_t AMMORecord::ParseRecord(unsigned char *buffer, unsigned char *end_buffer, bool CompressedOnDisk)
	{
		uint32_t subType = 0;
		uint32_t subSize = 0;
		StringLookups *LookupStrings = GetParentMod()->TES4.LookupStrings;
		while (buffer < end_buffer) {
			subType = *(uint32_t *)buffer;
			buffer += 4;
			switch (subType)
			{
			case REV32(XXXX):
				buffer += 2;
				subSize = *(uint32_t *)buffer;
				buffer += 4;
				subType = *(uint32_t *)buffer;
				buffer += 6;
				break;
			default:
				subSize = *(uint16_t *)buffer;
				buffer += 2;
				break;
			}
			switch (subType)
			{
			case REV32(EDID):
				EDID.Read(buffer, subSize, CompressedOnDisk);
				break;
			case REV32(OBND):
				OBND.Read(buffer, subSize);
				break;
			case REV32(FULL):
				FULL.Read(buffer, subSize, CompressedOnDisk, LookupStrings);
				break;
			case REV32(MODL):
				MODL.MODL.Read(buffer, subSize, CompressedOnDisk);
				break;
			case REV32(MODT):
				MODL.MODT.Read(buffer, subSize, CompressedOnDisk);
				break;
			case REV32(ICON):
				ICON.Read(buffer, subSize, CompressedOnDisk);
				break;
			case REV32(MICO):
				MICO.Read(buffer, subSize, CompressedOnDisk);
				break;
			case REV32(DEST):
				DEST.Read(buffer, subSize);
				break;
			case REV32(YNAM):
				YNAM.Read(buffer, subSize);
				break;
			case REV32(ZNAM):
				ZNAM.Read(buffer, subSize);
				break;
			case REV32(DESC):
				DESC.Read(buffer, subSize, CompressedOnDisk, LookupStrings);
				break;
			case REV32(KSIZ):
				// Ignore on read
				buffer += subSize;
				break;
			case REV32(KWDA):
				KWDA.Read(buffer, subSize);
				break;
			case REV32(DATA):
				DATA.Read(buffer, subSize);
				break;

			default:
				//printer("FileName = %s\n", FileName);
				printer("  AMMO: %08X - Unknown subType = %04x [%c%c%c%c]\n", formID, subType, (subType >> 0) & 0xFF, (subType >> 8) & 0xFF, (subType >> 16) & 0xFF, (subType >> 24) & 0xFF);
				CBASH_CHUNK_DEBUG
					printer("  Size = %i\n", subSize);
				printer("  CurPos = %08x\n\n", buffer - 6);
				buffer = end_buffer;
				break;
			}
		};
		return 0;
	}

	int32_t AMMORecord::Unload()
	{
		IsChanged(false);
		IsLoaded(false);
		EDID.Unload();
		OBND.Unload();
		FULL.Unload();
		ICON.Unload();
		MICO.Unload();
		DEST.Unload();
		YNAM.Unload();
		ZNAM.Unload();
		DESC.Unload();
		KWDA.Unload();
		DATA.Unload();

		return 1;
	}

	int32_t AMMORecord::WriteRecord(FileWriter &writer)
	{
		WRITE(EDID);
		WRITE(OBND);
		WRITE(FULL);
		MODL.Write(writer);
		WRITE(ICON);
		WRITE(MICO);
		WRITE(DEST);
		WRITE(YNAM);
		WRITE(ZNAM);
		WRITE(DESC);
		WRITE(KWDA);
		WRITE(DATA);
		return -1;
	}

	bool AMMORecord::operator ==(const AMMORecord &other) const
	{
		return (EDID.equalsi(other.EDID) &&
			OBND == other.OBND &&
			FULL.equalsi(other.FULL) &&
			MODL == other.MODL &&
			ICON.equalsi(other.ICON) &&
			MICO.equalsi(other.MICO) &&
			DEST == other.DEST &&
			YNAM == other.YNAM &&
			ZNAM == other.ZNAM &&
			DESC.equalsi(other.DESC) &&
			KWDA == other.KWDA &&
			DATA == other.DATA);
	}

	bool AMMORecord::operator !=(const AMMORecord &other) const
	{
		return !(*this == other);
	}

	bool AMMORecord::equals(Record *other)
	{
		return *this == *(AMMORecord *)other;
	}
}