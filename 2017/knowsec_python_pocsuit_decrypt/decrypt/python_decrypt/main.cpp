
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <vector>


using std::string;
using std::vector;


typedef struct {
    unsigned long  code_length;
    unsigned char* code_point;
} python_code;
typedef vector<python_code> python_code_segment;
/*
typedef struct {
    int knowsec_version_python_opcode;
    int source_version_python_opcode;
    int python_opcode_length;
} python_opcode_conver_;
typedef vector<python_opcode_conver_> python_opcode_conver_list_;

typedef struct {
    int source_version_python_opcode;
    int python_opcode_length;
} python_opcode_;
typedef vector<python_opcode_> python_opcode_list_;


python_opcode_conver_list_ python_opcode_conver_list;
python_opcode_list_        python_opcode_list;


void build_python_opcode_conver_list(void) {
    for (int build_source_version_python_opcode_index=0;build_source_version_python_opcode_index<=147;++build_source_version_python_opcode_index) {
        python_opcode_ source_version_python_opcode={0};

        source_version_python_opcode.source_version_python_opcode=build_source_version_python_opcode_index;
        source_version_python_opcode.python_opcode_length=(build_source_version_python_opcode_index<90)?0:2;
        python_opcode_list[build_source_version_python_opcode_index]=source_version_python_opcode;
    }

    python_opcode_conver_ python_opcode_conver={0};
    python_opcode_conver.knowsec_version_python_opcode=0xC9;
    python_opcode_conver.source_version_python_opcode=0x64;
    python_opcode_conver.python_opcode_length=3;
    python_opcode_conver_list.push_back(python_opcode_conver);
}
*/
unsigned char decrypt_key_buffer[]={116,190,118,89,127,180,51,246,199,10,245,25,18,4,174,217,171,42,50,39,152,168,160,75,103,33,188,105,220,214,236,12,161,29,159,95,98,76,137,131,224,163,93,191,194,74,26,143,37,16,38,135,102,123,65,240,239,54,255,201,84,132,173,238,46,136,7,157,212,192,9,134,183,251,28,79,112,172,242,205,0,110,177,211,87,36,21,19,17,8,49,60,176,207,73,142,253,115,178,27,64,53,179,196,181,193,189,101,47,235,3,202,94,151,97,128,78,228,156,106,162,158,82,45,153,254,66,185,234,56,167,55,122,223,195,133,62,114,204,81,67,107,232,13,70,149,91,20,124,119,85,247,44,165,203,237,222,175,210,24,209,184,77,96,120,14,147,144,215,197,229,5,61,1,117,83,182,99,30,216,72,166,169,125,68,148,90,63,218,198,41,145,250,57,35,111,226,249,141,155,208,121,213,139,221,244,100,233,104,129,48,11,113,32,241,219,43,140,243,80,59,231,92,206,69,88,227,200,130,31,22,58,225,52,186,146,164,109,138,108,2,230,15,40,150,34,187,252,23,126,86,154,6,248,170,71,0,0};


int decrypt(unsigned char* pyopcode_offset,unsigned char* decrypt_offset, unsigned int decrypt_length, unsigned char* decrypt_key_buffer) {
  unsigned int decrypt_index = 0; // rax@1
  unsigned char  first_decrypt_key_index = *(unsigned char*)(decrypt_key_buffer + 0x100); // r9@1
  unsigned char  second_decrypt_key_index = *(unsigned char*)(decrypt_key_buffer + 0x101); // r8@1
  unsigned char* second_decrypt_key_bit = NULL; // r10@3
  unsigned char  first_decrypt_key_bit = '\0'; // r11@3
  unsigned char* second_decrypt_key_offset = NULL; // rbx@3

  while ( decrypt_length > decrypt_index ) {
    second_decrypt_key_bit = (unsigned char*)(decrypt_key_buffer + (++first_decrypt_key_index));
    first_decrypt_key_bit = *second_decrypt_key_bit;
    second_decrypt_key_index += *second_decrypt_key_bit;
    second_decrypt_key_offset = (unsigned char*)(decrypt_key_buffer + second_decrypt_key_index);
    *second_decrypt_key_bit = *second_decrypt_key_offset;
    *second_decrypt_key_offset = first_decrypt_key_bit;

    unsigned char debug_bit_decrypy_key_offset=(unsigned char)(*second_decrypt_key_bit + first_decrypt_key_bit);
    unsigned char debug_bit_xor_right=*(unsigned char*)(decrypt_key_buffer + debug_bit_decrypy_key_offset);
    unsigned char debug_bit_decrypt_key=*(unsigned char*)(decrypt_offset + decrypt_index) ^ debug_bit_xor_right;

    *(unsigned char*)(pyopcode_offset + decrypt_index) = debug_bit_decrypt_key;

    ++decrypt_index;
  }

  *(unsigned char*)(decrypt_key_buffer + 0x100) = first_decrypt_key_index;
  *(unsigned char*)(decrypt_key_buffer + 0x101) = second_decrypt_key_index;

  return decrypt_index;
}

void PyShield_Decrypt(unsigned char* PyOpCode,unsigned long decrypt_length) {
  unsigned char decrypt_key[0x102] = {0}; // [sp+0h] [bp-128h]@2

  memcpy(decrypt_key,decrypt_key_buffer,0x100);

  decrypt(PyOpCode, PyOpCode, decrypt_length, (unsigned char*)&decrypt_key);
}

void conver_to_normal_opcode(unsigned char* PyOpCode,unsigned long conver_length) {
    for (unsigned long conver_index=0;conver_index<conver_length;) {
        unsigned char python_opcode=*(PyOpCode+conver_index);

        switch (python_opcode) {
            case 0xC9 : {
                *(PyOpCode+conver_index)=0x64;
                conver_index+=3;
                break;
            } case 0xCA : {
                *(PyOpCode+conver_index)=0x74;
                conver_index+=3;
                break;
            } case 0xC8 : {
                *(PyOpCode+conver_index)=0x7C;
                conver_index+=3;
                break;
            } default : {
                if (90<=python_opcode)  //  this opcode need to take tow arguments ..
                    conver_index+=3;
                else
                    conver_index+=1;
            }
        }
    }
}


unsigned long get_file_size(char* file_path) {
    FILE* file=fopen(file_path,"rb+");

    fseek(file,0,SEEK_END);

    unsigned long file_size=(unsigned long)ftell(file);

    fclose(file);

    return file_size;
}

unsigned char* read_file(char* file_path) {
    FILE* file=fopen(file_path,"rb+");

    if (NULL!=file) {
        fseek(file,0,SEEK_END);

        unsigned long  file_size=(unsigned long)ftell(file);
        unsigned char* file_data=(unsigned char*)malloc(file_size);

        memset(file_data,0,file_size);
        fseek(file,0,SEEK_SET);
        fread(file_data,1,file_size,file);
        fclose(file);

        return file_data;
    }
    return NULL;
}

void save_file(char* file_path,unsigned char* file_data,unsigned long file_data_length) {
#define SAVE_EXTERN_NAME ".decrypt.pyc"

    unsigned long file_path_length=strlen(file_path)+strlen(SAVE_EXTERN_NAME)+1;
    char* new_file_path=(char*)malloc(file_path_length);

    memcpy(new_file_path,file_path,strlen(file_path));
    memcpy((void*)(new_file_path+strlen(file_path)),SAVE_EXTERN_NAME,strlen(SAVE_EXTERN_NAME));

    new_file_path[file_path_length]='\0';

    FILE* file=fopen(new_file_path,"wb");

    fwrite((const void*)file_data,1,file_data_length,file);
    fclose(file);
}

unsigned char* find_code_segment_flag(unsigned char* block_offset,unsigned char* block_end) {
    for (;block_offset<block_end;++block_offset) {
        if (0x63==*block_offset) {
            unsigned char* inside_block_offset=&block_offset[sizeof(unsigned long)*4+1];

            if (0x73==*inside_block_offset) {
                try {
                    unsigned long  valid_code_size=*(unsigned long*)&inside_block_offset[1];

                    inside_block_offset+=sizeof(unsigned long)+1;

                    if (0x28==inside_block_offset[valid_code_size])
                        return block_offset;
                } catch (...) {
                }
            }
        }
    }

    return NULL;
}

python_code_segment resolve_python_file(unsigned char* file_data,unsigned long file_size) {
    unsigned long  magic_number=*(unsigned long*)&file_data[0];
    unsigned long  time_number=*(unsigned long*)&file_data[4];
    unsigned char* block_offset=(unsigned char*)&file_data[sizeof(unsigned long)*2];
    unsigned char* block_offset_end=(unsigned char*)(file_data+file_size);
    python_code_segment return_result;

    while (NULL!=(block_offset=find_code_segment_flag(block_offset,block_offset_end))) {
        if (0x63==*block_offset) {
            block_offset+=sizeof(unsigned long)*4+1;

            unsigned char code_block_type=*(unsigned char*)block_offset++;

            if (0x73==code_block_type) {
                unsigned long code_block_size=*(unsigned long*)block_offset;

                block_offset+=sizeof(unsigned long);

                if (0x28==block_offset[code_block_size]) {
                    python_code python_code_={0};
                    python_code_.code_length=code_block_size;
                    python_code_.code_point=block_offset;
                    block_offset+=code_block_size+5;

                    return_result.push_back(python_code_);
                }
            }
        } else {
            break;
        }
    }

    return return_result;
}

void decrypt_pyc(char* pyc_path) {
    unsigned char* file_data=read_file(pyc_path);
    unsigned long  file_size=get_file_size(pyc_path);

    python_code_segment pyc_file_code_segment=resolve_python_file(file_data,file_size);

    for (python_code_segment::iterator pyc_file_code_segment_index=pyc_file_code_segment.begin();
                                       pyc_file_code_segment_index!=pyc_file_code_segment.end();
                                       ++pyc_file_code_segment_index) {
        PyShield_Decrypt(pyc_file_code_segment_index->code_point,pyc_file_code_segment_index->code_length);
        conver_to_normal_opcode(pyc_file_code_segment_index->code_point,pyc_file_code_segment_index->code_length);
        printf("Decrypt Address 0x%X (0x%X) \\n",pyc_file_code_segment_index->code_point,pyc_file_code_segment_index->code_length);
    }

    save_file(pyc_path,file_data,file_size);
}

void main(unsigned int argc,char** argv) {
    if (2==argc) {
        decrypt_pyc(argv[1]);
    } else {
        printf("take a .pyc path ..");
    }
}
