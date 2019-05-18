/*
        Copyright 2013 to 2017 TeamWin
        This file is part of TWRP/TeamWin Recovery Project.

        TWRP is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        TWRP is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with TWRP.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __TWRP_DIGEST_DRIVER
#define __TWRP_DIGEST_DRIVER
#include <string>
#include "twrpDigest/twrpDigest.hpp"

enum Digest_Type {
        MD5 = 0,
        SHA2
};

class twrpDigestDriver {
public:

        bool Check_File_Digest(const string& Filename);		         // Check the digest of a TWRP partition backup
	bool Check_Digest(string Full_Filename);			 // Check to make sure the digest is correct
	bool Write_Digest(string Full_Filename);	                 // Write the digest to a file
	bool Make_Digest(string Full_Filename);				 // Create the digest for a partition backup
	bool stream_file_to_digest(string filename, twrpDigest* digest); // Stream the file to twrpDigest
        void set_digest_type(Digest_Type digest_type);                   // Set and create digest type to use
        void stream_and_update_digest(char* buf);                        // Update digest for stream
        void close_digest();                                             // Close and delete digest
private:
        twrpDigest *digest;                                              // Store the digest for twrpDigestDriver
        enum Digest_Type current_digest_type;                            // Store current digest type to create appropriate digest
};
#endif //__TWRP_DIGEST_DRIVER
