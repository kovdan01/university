#include "shared.hpp"
#include <iostream>

static const table_t DECRYPTION_TABLE =
{
    mask_t{ 0x6c, 0x72, 0x69, 0x38, 0x33, 0x6e, 0x76, 0x37, 0x6b, 0x77, 0x6b, 0x77, 0x65, 0x34, 0x75, 0x33, },
    mask_t{ 0x6d, 0x73, 0x68, 0x39, 0x32, 0x6f, 0x77, 0x36, 0x6a, 0x76, 0x6a, 0x76, 0x64, 0x35, 0x74, 0x32, },
    mask_t{ 0x6e, 0x70, 0x6b, 0x3a, 0x31, 0x6c, 0x74, 0x35, 0x69, 0x75, 0x69, 0x75, 0x67, 0x36, 0x77, 0x31, },
    mask_t{ 0x6f, 0x71, 0x6a, 0x3b, 0x30, 0x6d, 0x75, 0x34, 0x68, 0x74, 0x68, 0x74, 0x66, 0x37, 0x76, 0x30, },
    mask_t{ 0x68, 0x76, 0x6d, 0x3c, 0x37, 0x6a, 0x72, 0x33, 0x6f, 0x73, 0x6f, 0x73, 0x61, 0x30, 0x71, 0x37, },
    mask_t{ 0x69, 0x77, 0x6c, 0x3d, 0x36, 0x6b, 0x73, 0x32, 0x6e, 0x72, 0x6e, 0x72, 0x60, 0x31, 0x70, 0x36, },
    mask_t{ 0x6a, 0x74, 0x6f, 0x3e, 0x35, 0x68, 0x70, 0x31, 0x6d, 0x71, 0x6d, 0x71, 0x63, 0x32, 0x73, 0x35, },
    mask_t{ 0x6b, 0x75, 0x6e, 0x3f, 0x34, 0x69, 0x71, 0x30, 0x6c, 0x70, 0x6c, 0x70, 0x62, 0x33, 0x72, 0x34, },
    mask_t{ 0x64, 0x7a, 0x61, 0x30, 0x3b, 0x66, 0x7e, 0x3f, 0x63, 0x7f, 0x63, 0x7f, 0x6d, 0x3c, 0x7d, 0x3b, },
    mask_t{ 0x65, 0x7b, 0x60, 0x31, 0x3a, 0x67, 0x7f, 0x3e, 0x62, 0x7e, 0x62, 0x7e, 0x6c, 0x3d, 0x7c, 0x3a, },
    mask_t{ 0x66, 0x78, 0x63, 0x32, 0x39, 0x64, 0x7c, 0x3d, 0x61, 0x7d, 0x61, 0x7d, 0x6f, 0x3e, 0x7f, 0x39, },
    mask_t{ 0x67, 0x79, 0x62, 0x33, 0x38, 0x65, 0x7d, 0x3c, 0x60, 0x7c, 0x60, 0x7c, 0x6e, 0x3f, 0x7e, 0x38, },
    mask_t{ 0x60, 0x7e, 0x65, 0x34, 0x3f, 0x62, 0x7a, 0x3b, 0x67, 0x7b, 0x67, 0x7b, 0x69, 0x38, 0x79, 0x3f, },
    mask_t{ 0x61, 0x7f, 0x64, 0x35, 0x3e, 0x63, 0x7b, 0x3a, 0x66, 0x7a, 0x66, 0x7a, 0x68, 0x39, 0x78, 0x3e, },
    mask_t{ 0x62, 0x7c, 0x67, 0x36, 0x3d, 0x60, 0x78, 0x39, 0x65, 0x79, 0x65, 0x79, 0x6b, 0x3a, 0x7b, 0x3d, },
    mask_t{ 0x63, 0x7d, 0x66, 0x37, 0x3c, 0x61, 0x79, 0x38, 0x64, 0x78, 0x64, 0x78, 0x6a, 0x3b, 0x7a, 0x3c, },
    mask_t{ 0x7c, 0x62, 0x79, 0x28, 0x23, 0x7e, 0x66, 0x27, 0x7b, 0x67, 0x7b, 0x67, 0x75, 0x24, 0x65, 0x23, },
    mask_t{ 0x7d, 0x63, 0x78, 0x29, 0x22, 0x7f, 0x67, 0x26, 0x7a, 0x66, 0x7a, 0x66, 0x74, 0x25, 0x64, 0x22, },
    mask_t{ 0x7e, 0x60, 0x7b, 0x2a, 0x21, 0x7c, 0x64, 0x25, 0x79, 0x65, 0x79, 0x65, 0x77, 0x26, 0x67, 0x21, },
    mask_t{ 0x7f, 0x61, 0x7a, 0x2b, 0x20, 0x7d, 0x65, 0x24, 0x78, 0x64, 0x78, 0x64, 0x76, 0x27, 0x66, 0x20, },
    mask_t{ 0x78, 0x66, 0x7d, 0x2c, 0x27, 0x7a, 0x62, 0x23, 0x7f, 0x63, 0x7f, 0x63, 0x71, 0x20, 0x61, 0x27, },
    mask_t{ 0x79, 0x67, 0x7c, 0x2d, 0x26, 0x7b, 0x63, 0x22, 0x7e, 0x62, 0x7e, 0x62, 0x70, 0x21, 0x60, 0x26, },
    mask_t{ 0x7a, 0x64, 0x7f, 0x2e, 0x25, 0x78, 0x60, 0x21, 0x7d, 0x61, 0x7d, 0x61, 0x73, 0x22, 0x63, 0x25, },
    mask_t{ 0x7b, 0x65, 0x7e, 0x2f, 0x24, 0x79, 0x61, 0x20, 0x7c, 0x60, 0x7c, 0x60, 0x72, 0x23, 0x62, 0x24, },
    mask_t{ 0x74, 0x6a, 0x71, 0x20, 0x2b, 0x76, 0x6e, 0x2f, 0x73, 0x6f, 0x73, 0x6f, 0x7d, 0x2c, 0x6d, 0x2b, },
    mask_t{ 0x75, 0x6b, 0x70, 0x21, 0x2a, 0x77, 0x6f, 0x2e, 0x72, 0x6e, 0x72, 0x6e, 0x7c, 0x2d, 0x6c, 0x2a, },
    mask_t{ 0x76, 0x68, 0x73, 0x22, 0x29, 0x74, 0x6c, 0x2d, 0x71, 0x6d, 0x71, 0x6d, 0x7f, 0x2e, 0x6f, 0x29, },
    mask_t{ 0x77, 0x69, 0x72, 0x23, 0x28, 0x75, 0x6d, 0x2c, 0x70, 0x6c, 0x70, 0x6c, 0x7e, 0x2f, 0x6e, 0x28, },
    mask_t{ 0x70, 0x6e, 0x75, 0x24, 0x2f, 0x72, 0x6a, 0x2b, 0x77, 0x6b, 0x77, 0x6b, 0x79, 0x28, 0x69, 0x2f, },
    mask_t{ 0x71, 0x6f, 0x74, 0x25, 0x2e, 0x73, 0x6b, 0x2a, 0x76, 0x6a, 0x76, 0x6a, 0x78, 0x29, 0x68, 0x2e, },
    mask_t{ 0x72, 0x6c, 0x77, 0x26, 0x2d, 0x70, 0x68, 0x29, 0x75, 0x69, 0x75, 0x69, 0x7b, 0x2a, 0x6b, 0x2d, },
    mask_t{ 0x73, 0x6d, 0x76, 0x27, 0x2c, 0x71, 0x69, 0x28, 0x74, 0x68, 0x74, 0x68, 0x7a, 0x2b, 0x6a, 0x2c, },
    mask_t{ 0x4c, 0x52, 0x49, 0x18, 0x13, 0x4e, 0x56, 0x17, 0x4b, 0x57, 0x4b, 0x57, 0x45, 0x14, 0x55, 0x13, },
    mask_t{ 0x4d, 0x53, 0x48, 0x19, 0x12, 0x4f, 0x57, 0x16, 0x4a, 0x56, 0x4a, 0x56, 0x44, 0x15, 0x54, 0x12, },
    mask_t{ 0x4e, 0x50, 0x4b, 0x1a, 0x11, 0x4c, 0x54, 0x15, 0x49, 0x55, 0x49, 0x55, 0x47, 0x16, 0x57, 0x11, },
    mask_t{ 0x4f, 0x51, 0x4a, 0x1b, 0x10, 0x4d, 0x55, 0x14, 0x48, 0x54, 0x48, 0x54, 0x46, 0x17, 0x56, 0x10, },
    mask_t{ 0x48, 0x56, 0x4d, 0x1c, 0x17, 0x4a, 0x52, 0x13, 0x4f, 0x53, 0x4f, 0x53, 0x41, 0x10, 0x51, 0x17, },
    mask_t{ 0x49, 0x57, 0x4c, 0x1d, 0x16, 0x4b, 0x53, 0x12, 0x4e, 0x52, 0x4e, 0x52, 0x40, 0x11, 0x50, 0x16, },
    mask_t{ 0x4a, 0x54, 0x4f, 0x1e, 0x15, 0x48, 0x50, 0x11, 0x4d, 0x51, 0x4d, 0x51, 0x43, 0x12, 0x53, 0x15, },
    mask_t{ 0x4b, 0x55, 0x4e, 0x1f, 0x14, 0x49, 0x51, 0x10, 0x4c, 0x50, 0x4c, 0x50, 0x42, 0x13, 0x52, 0x14, },
    mask_t{ 0x44, 0x5a, 0x41, 0x10, 0x1b, 0x46, 0x5e, 0x1f, 0x43, 0x5f, 0x43, 0x5f, 0x4d, 0x1c, 0x5d, 0x1b, },
    mask_t{ 0x45, 0x5b, 0x40, 0x11, 0x1a, 0x47, 0x5f, 0x1e, 0x42, 0x5e, 0x42, 0x5e, 0x4c, 0x1d, 0x5c, 0x1a, },
    mask_t{ 0x46, 0x58, 0x43, 0x12, 0x19, 0x44, 0x5c, 0x1d, 0x41, 0x5d, 0x41, 0x5d, 0x4f, 0x1e, 0x5f, 0x19, },
    mask_t{ 0x47, 0x59, 0x42, 0x13, 0x18, 0x45, 0x5d, 0x1c, 0x40, 0x5c, 0x40, 0x5c, 0x4e, 0x1f, 0x5e, 0x18, },
    mask_t{ 0x40, 0x5e, 0x45, 0x14, 0x1f, 0x42, 0x5a, 0x1b, 0x47, 0x5b, 0x47, 0x5b, 0x49, 0x18, 0x59, 0x1f, },
    mask_t{ 0x41, 0x5f, 0x44, 0x15, 0x1e, 0x43, 0x5b, 0x1a, 0x46, 0x5a, 0x46, 0x5a, 0x48, 0x19, 0x58, 0x1e, },
    mask_t{ 0x42, 0x5c, 0x47, 0x16, 0x1d, 0x40, 0x58, 0x19, 0x45, 0x59, 0x45, 0x59, 0x4b, 0x1a, 0x5b, 0x1d, },
    mask_t{ 0x43, 0x5d, 0x46, 0x17, 0x1c, 0x41, 0x59, 0x18, 0x44, 0x58, 0x44, 0x58, 0x4a, 0x1b, 0x5a, 0x1c, },
    mask_t{ 0x5c, 0x42, 0x59, 0x08, 0x03, 0x5e, 0x46, 0x07, 0x5b, 0x47, 0x5b, 0x47, 0x55, 0x04, 0x45, 0x03, },
    mask_t{ 0x5d, 0x43, 0x58, 0x09, 0x02, 0x5f, 0x47, 0x06, 0x5a, 0x46, 0x5a, 0x46, 0x54, 0x05, 0x44, 0x02, },
    mask_t{ 0x5e, 0x40, 0x5b, 0x0a, 0x01, 0x5c, 0x44, 0x05, 0x59, 0x45, 0x59, 0x45, 0x57, 0x06, 0x47, 0x01, },
    mask_t{ 0x5f, 0x41, 0x5a, 0x0b, 0000, 0x5d, 0x45, 0x04, 0x58, 0x44, 0x58, 0x44, 0x56, 0x07, 0x46, 0000, },
    mask_t{ 0x58, 0x46, 0x5d, 0x0c, 0x07, 0x5a, 0x42, 0x03, 0x5f, 0x43, 0x5f, 0x43, 0x51, 0000, 0x41, 0x07, },
    mask_t{ 0x59, 0x47, 0x5c, 0x0d, 0x06, 0x5b, 0x43, 0x02, 0x5e, 0x42, 0x5e, 0x42, 0x50, 0x01, 0x40, 0x06, },
    mask_t{ 0x5a, 0x44, 0x5f, 0x0e, 0x05, 0x58, 0x40, 0x01, 0x5d, 0x41, 0x5d, 0x41, 0x53, 0x02, 0x43, 0x05, },
    mask_t{ 0x5b, 0x45, 0x5e, 0x0f, 0x04, 0x59, 0x41, 0000, 0x5c, 0x40, 0x5c, 0x40, 0x52, 0x03, 0x42, 0x04, },
    mask_t{ 0x54, 0x4a, 0x51, 0000, 0x0b, 0x56, 0x4e, 0x0f, 0x53, 0x4f, 0x53, 0x4f, 0x5d, 0x0c, 0x4d, 0x0b, },
    mask_t{ 0x55, 0x4b, 0x50, 0x01, 0x0a, 0x57, 0x4f, 0x0e, 0x52, 0x4e, 0x52, 0x4e, 0x5c, 0x0d, 0x4c, 0x0a, },
    mask_t{ 0x56, 0x48, 0x53, 0x02, 0x09, 0x54, 0x4c, 0x0d, 0x51, 0x4d, 0x51, 0x4d, 0x5f, 0x0e, 0x4f, 0x09, },
    mask_t{ 0x57, 0x49, 0x52, 0x03, 0x08, 0x55, 0x4d, 0x0c, 0x50, 0x4c, 0x50, 0x4c, 0x5e, 0x0f, 0x4e, 0x08, },
    mask_t{ 0x50, 0x4e, 0x55, 0x04, 0x0f, 0x52, 0x4a, 0x0b, 0x57, 0x4b, 0x57, 0x4b, 0x59, 0x08, 0x49, 0x0f, },
    mask_t{ 0x51, 0x4f, 0x54, 0x05, 0x0e, 0x53, 0x4b, 0x0a, 0x56, 0x4a, 0x56, 0x4a, 0x58, 0x09, 0x48, 0x0e, },
    mask_t{ 0x52, 0x4c, 0x57, 0x06, 0x0d, 0x50, 0x48, 0x09, 0x55, 0x49, 0x55, 0x49, 0x5b, 0x0a, 0x4b, 0x0d, },
    mask_t{ 0x53, 0x4d, 0x56, 0x07, 0x0c, 0x51, 0x49, 0x08, 0x54, 0x48, 0x54, 0x48, 0x5a, 0x0b, 0x4a, 0x0c, },
    mask_t{ 0x2c, 0x32, 0x29, 0x78, 0x73, 0x2e, 0x36, 0x77, 0x2b, 0x37, 0x2b, 0x37, 0x25, 0x74, 0x35, 0x73, },
    mask_t{ 0x2d, 0x33, 0x28, 0x79, 0x72, 0x2f, 0x37, 0x76, 0x2a, 0x36, 0x2a, 0x36, 0x24, 0x75, 0x34, 0x72, },
    mask_t{ 0x2e, 0x30, 0x2b, 0x7a, 0x71, 0x2c, 0x34, 0x75, 0x29, 0x35, 0x29, 0x35, 0x27, 0x76, 0x37, 0x71, },
    mask_t{ 0x2f, 0x31, 0x2a, 0x7b, 0x70, 0x2d, 0x35, 0x74, 0x28, 0x34, 0x28, 0x34, 0x26, 0x77, 0x36, 0x70, },
    mask_t{ 0x28, 0x36, 0x2d, 0x7c, 0x77, 0x2a, 0x32, 0x73, 0x2f, 0x33, 0x2f, 0x33, 0x21, 0x70, 0x31, 0x77, },
    mask_t{ 0x29, 0x37, 0x2c, 0x7d, 0x76, 0x2b, 0x33, 0x72, 0x2e, 0x32, 0x2e, 0x32, 0x20, 0x71, 0x30, 0x76, },
    mask_t{ 0x2a, 0x34, 0x2f, 0x7e, 0x75, 0x28, 0x30, 0x71, 0x2d, 0x31, 0x2d, 0x31, 0x23, 0x72, 0x33, 0x75, },
    mask_t{ 0x2b, 0x35, 0x2e, 0x7f, 0x74, 0x29, 0x31, 0x70, 0x2c, 0x30, 0x2c, 0x30, 0x22, 0x73, 0x32, 0x74, },
    mask_t{ 0x24, 0x3a, 0x21, 0x70, 0x7b, 0x26, 0x3e, 0x7f, 0x23, 0x3f, 0x23, 0x3f, 0x2d, 0x7c, 0x3d, 0x7b, },
    mask_t{ 0x25, 0x3b, 0x20, 0x71, 0x7a, 0x27, 0x3f, 0x7e, 0x22, 0x3e, 0x22, 0x3e, 0x2c, 0x7d, 0x3c, 0x7a, },
    mask_t{ 0x26, 0x38, 0x23, 0x72, 0x79, 0x24, 0x3c, 0x7d, 0x21, 0x3d, 0x21, 0x3d, 0x2f, 0x7e, 0x3f, 0x79, },
    mask_t{ 0x27, 0x39, 0x22, 0x73, 0x78, 0x25, 0x3d, 0x7c, 0x20, 0x3c, 0x20, 0x3c, 0x2e, 0x7f, 0x3e, 0x78, },
    mask_t{ 0x20, 0x3e, 0x25, 0x74, 0x7f, 0x22, 0x3a, 0x7b, 0x27, 0x3b, 0x27, 0x3b, 0x29, 0x78, 0x39, 0x7f, },
    mask_t{ 0x21, 0x3f, 0x24, 0x75, 0x7e, 0x23, 0x3b, 0x7a, 0x26, 0x3a, 0x26, 0x3a, 0x28, 0x79, 0x38, 0x7e, },
    mask_t{ 0x22, 0x3c, 0x27, 0x76, 0x7d, 0x20, 0x38, 0x79, 0x25, 0x39, 0x25, 0x39, 0x2b, 0x7a, 0x3b, 0x7d, },
    mask_t{ 0x23, 0x3d, 0x26, 0x77, 0x7c, 0x21, 0x39, 0x78, 0x24, 0x38, 0x24, 0x38, 0x2a, 0x7b, 0x3a, 0x7c, },
    mask_t{ 0x3c, 0x22, 0x39, 0x68, 0x63, 0x3e, 0x26, 0x67, 0x3b, 0x27, 0x3b, 0x27, 0x35, 0x64, 0x25, 0x63, },
    mask_t{ 0x3d, 0x23, 0x38, 0x69, 0x62, 0x3f, 0x27, 0x66, 0x3a, 0x26, 0x3a, 0x26, 0x34, 0x65, 0x24, 0x62, },
    mask_t{ 0x3e, 0x20, 0x3b, 0x6a, 0x61, 0x3c, 0x24, 0x65, 0x39, 0x25, 0x39, 0x25, 0x37, 0x66, 0x27, 0x61, },
    mask_t{ 0x3f, 0x21, 0x3a, 0x6b, 0x60, 0x3d, 0x25, 0x64, 0x38, 0x24, 0x38, 0x24, 0x36, 0x67, 0x26, 0x60, },
    mask_t{ 0x38, 0x26, 0x3d, 0x6c, 0x67, 0x3a, 0x22, 0x63, 0x3f, 0x23, 0x3f, 0x23, 0x31, 0x60, 0x21, 0x67, },
    mask_t{ 0x39, 0x27, 0x3c, 0x6d, 0x66, 0x3b, 0x23, 0x62, 0x3e, 0x22, 0x3e, 0x22, 0x30, 0x61, 0x20, 0x66, },
    mask_t{ 0x3a, 0x24, 0x3f, 0x6e, 0x65, 0x38, 0x20, 0x61, 0x3d, 0x21, 0x3d, 0x21, 0x33, 0x62, 0x23, 0x65, },
    mask_t{ 0x3b, 0x25, 0x3e, 0x6f, 0x64, 0x39, 0x21, 0x60, 0x3c, 0x20, 0x3c, 0x20, 0x32, 0x63, 0x22, 0x64, },
    mask_t{ 0x34, 0x2a, 0x31, 0x60, 0x6b, 0x36, 0x2e, 0x6f, 0x33, 0x2f, 0x33, 0x2f, 0x3d, 0x6c, 0x2d, 0x6b, },
    mask_t{ 0x35, 0x2b, 0x30, 0x61, 0x6a, 0x37, 0x2f, 0x6e, 0x32, 0x2e, 0x32, 0x2e, 0x3c, 0x6d, 0x2c, 0x6a, },
    mask_t{ 0x36, 0x28, 0x33, 0x62, 0x69, 0x34, 0x2c, 0x6d, 0x31, 0x2d, 0x31, 0x2d, 0x3f, 0x6e, 0x2f, 0x69, },
    mask_t{ 0x37, 0x29, 0x32, 0x63, 0x68, 0x35, 0x2d, 0x6c, 0x30, 0x2c, 0x30, 0x2c, 0x3e, 0x6f, 0x2e, 0x68, },
    mask_t{ 0x30, 0x2e, 0x35, 0x64, 0x6f, 0x32, 0x2a, 0x6b, 0x37, 0x2b, 0x37, 0x2b, 0x39, 0x68, 0x29, 0x6f, },
    mask_t{ 0x31, 0x2f, 0x34, 0x65, 0x6e, 0x33, 0x2b, 0x6a, 0x36, 0x2a, 0x36, 0x2a, 0x38, 0x69, 0x28, 0x6e, },
    mask_t{ 0x32, 0x2c, 0x37, 0x66, 0x6d, 0x30, 0x28, 0x69, 0x35, 0x29, 0x35, 0x29, 0x3b, 0x6a, 0x2b, 0x6d, },
    mask_t{ 0x33, 0x2d, 0x36, 0x67, 0x6c, 0x31, 0x29, 0x68, 0x34, 0x28, 0x34, 0x28, 0x3a, 0x6b, 0x2a, 0x6c, },
    mask_t{ 0x0c, 0x12, 0x09, 0x58, 0x53, 0x0e, 0x16, 0x57, 0x0b, 0x17, 0x0b, 0x17, 0x05, 0x54, 0x15, 0x53, },
    mask_t{ 0x0d, 0x13, 0x08, 0x59, 0x52, 0x0f, 0x17, 0x56, 0x0a, 0x16, 0x0a, 0x16, 0x04, 0x55, 0x14, 0x52, },
    mask_t{ 0x0e, 0x10, 0x0b, 0x5a, 0x51, 0x0c, 0x14, 0x55, 0x09, 0x15, 0x09, 0x15, 0x07, 0x56, 0x17, 0x51, },
    mask_t{ 0x0f, 0x11, 0x0a, 0x5b, 0x50, 0x0d, 0x15, 0x54, 0x08, 0x14, 0x08, 0x14, 0x06, 0x57, 0x16, 0x50, },
    mask_t{ 0x08, 0x16, 0x0d, 0x5c, 0x57, 0x0a, 0x12, 0x53, 0x0f, 0x13, 0x0f, 0x13, 0x01, 0x50, 0x11, 0x57, },
    mask_t{ 0x09, 0x17, 0x0c, 0x5d, 0x56, 0x0b, 0x13, 0x52, 0x0e, 0x12, 0x0e, 0x12, 0000, 0x51, 0x10, 0x56, },
    mask_t{ 0x0a, 0x14, 0x0f, 0x5e, 0x55, 0x08, 0x10, 0x51, 0x0d, 0x11, 0x0d, 0x11, 0x03, 0x52, 0x13, 0x55, },
    mask_t{ 0x0b, 0x15, 0x0e, 0x5f, 0x54, 0x09, 0x11, 0x50, 0x0c, 0x10, 0x0c, 0x10, 0x02, 0x53, 0x12, 0x54, },
    mask_t{ 0x04, 0x1a, 0x01, 0x50, 0x5b, 0x06, 0x1e, 0x5f, 0x03, 0x1f, 0x03, 0x1f, 0x0d, 0x5c, 0x1d, 0x5b, },
    mask_t{ 0x05, 0x1b, 0000, 0x51, 0x5a, 0x07, 0x1f, 0x5e, 0x02, 0x1e, 0x02, 0x1e, 0x0c, 0x5d, 0x1c, 0x5a, },
    mask_t{ 0x06, 0x18, 0x03, 0x52, 0x59, 0x04, 0x1c, 0x5d, 0x01, 0x1d, 0x01, 0x1d, 0x0f, 0x5e, 0x1f, 0x59, },
    mask_t{ 0x07, 0x19, 0x02, 0x53, 0x58, 0x05, 0x1d, 0x5c, 0000, 0x1c, 0000, 0x1c, 0x0e, 0x5f, 0x1e, 0x58, },
    mask_t{ 0000, 0x1e, 0x05, 0x54, 0x5f, 0x02, 0x1a, 0x5b, 0x07, 0x1b, 0x07, 0x1b, 0x09, 0x58, 0x19, 0x5f, },
    mask_t{ 0x01, 0x1f, 0x04, 0x55, 0x5e, 0x03, 0x1b, 0x5a, 0x06, 0x1a, 0x06, 0x1a, 0x08, 0x59, 0x18, 0x5e, },
    mask_t{ 0x02, 0x1c, 0x07, 0x56, 0x5d, 0000, 0x18, 0x59, 0x05, 0x19, 0x05, 0x19, 0x0b, 0x5a, 0x1b, 0x5d, },
    mask_t{ 0x03, 0x1d, 0x06, 0x57, 0x5c, 0x01, 0x19, 0x58, 0x04, 0x18, 0x04, 0x18, 0x0a, 0x5b, 0x1a, 0x5c, },
    mask_t{ 0x1c, 0x02, 0x19, 0x48, 0x43, 0x1e, 0x06, 0x47, 0x1b, 0x07, 0x1b, 0x07, 0x15, 0x44, 0x05, 0x43, },
    mask_t{ 0x1d, 0x03, 0x18, 0x49, 0x42, 0x1f, 0x07, 0x46, 0x1a, 0x06, 0x1a, 0x06, 0x14, 0x45, 0x04, 0x42, },
    mask_t{ 0x1e, 0000, 0x1b, 0x4a, 0x41, 0x1c, 0x04, 0x45, 0x19, 0x05, 0x19, 0x05, 0x17, 0x46, 0x07, 0x41, },
    mask_t{ 0x1f, 0x01, 0x1a, 0x4b, 0x40, 0x1d, 0x05, 0x44, 0x18, 0x04, 0x18, 0x04, 0x16, 0x47, 0x06, 0x40, },
    mask_t{ 0x18, 0x06, 0x1d, 0x4c, 0x47, 0x1a, 0x02, 0x43, 0x1f, 0x03, 0x1f, 0x03, 0x11, 0x40, 0x01, 0x47, },
    mask_t{ 0x19, 0x07, 0x1c, 0x4d, 0x46, 0x1b, 0x03, 0x42, 0x1e, 0x02, 0x1e, 0x02, 0x10, 0x41, 0000, 0x46, },
    mask_t{ 0x1a, 0x04, 0x1f, 0x4e, 0x45, 0x18, 0000, 0x41, 0x1d, 0x01, 0x1d, 0x01, 0x13, 0x42, 0x03, 0x45, },
    mask_t{ 0x1b, 0x05, 0x1e, 0x4f, 0x44, 0x19, 0x01, 0x40, 0x1c, 0000, 0x1c, 0000, 0x12, 0x43, 0x02, 0x44, },
    mask_t{ 0x14, 0x0a, 0x11, 0x40, 0x4b, 0x16, 0x0e, 0x4f, 0x13, 0x0f, 0x13, 0x0f, 0x1d, 0x4c, 0x0d, 0x4b, },
    mask_t{ 0x15, 0x0b, 0x10, 0x41, 0x4a, 0x17, 0x0f, 0x4e, 0x12, 0x0e, 0x12, 0x0e, 0x1c, 0x4d, 0x0c, 0x4a, },
    mask_t{ 0x16, 0x08, 0x13, 0x42, 0x49, 0x14, 0x0c, 0x4d, 0x11, 0x0d, 0x11, 0x0d, 0x1f, 0x4e, 0x0f, 0x49, },
    mask_t{ 0x17, 0x09, 0x12, 0x43, 0x48, 0x15, 0x0d, 0x4c, 0x10, 0x0c, 0x10, 0x0c, 0x1e, 0x4f, 0x0e, 0x48, },
    mask_t{ 0x10, 0x0e, 0x15, 0x44, 0x4f, 0x12, 0x0a, 0x4b, 0x17, 0x0b, 0x17, 0x0b, 0x19, 0x48, 0x09, 0x4f, },
    mask_t{ 0x11, 0x0f, 0x14, 0x45, 0x4e, 0x13, 0x0b, 0x4a, 0x16, 0x0a, 0x16, 0x0a, 0x18, 0x49, 0x08, 0x4e, },
    mask_t{ 0x12, 0x0c, 0x17, 0x46, 0x4d, 0x10, 0x08, 0x49, 0x15, 0x09, 0x15, 0x09, 0x1b, 0x4a, 0x0b, 0x4d, },
    mask_t{ 0x13, 0x0d, 0x16, 0x47, 0x4c, 0x11, 0x09, 0x48, 0x14, 0x08, 0x14, 0x08, 0x1a, 0x4b, 0x0a, 0x4c, },
    mask_t{ 0xec, 0xf2, 0xe9, 0xb8, 0xb3, 0xee, 0xf6, 0xb7, 0xeb, 0xf7, 0xeb, 0xf7, 0xe5, 0xb4, 0xf5, 0xb3, },
    mask_t{ 0xed, 0xf3, 0xe8, 0xb9, 0xb2, 0xef, 0xf7, 0xb6, 0xea, 0xf6, 0xea, 0xf6, 0xe4, 0xb5, 0xf4, 0xb2, },
    mask_t{ 0xee, 0xf0, 0xeb, 0xba, 0xb1, 0xec, 0xf4, 0xb5, 0xe9, 0xf5, 0xe9, 0xf5, 0xe7, 0xb6, 0xf7, 0xb1, },
    mask_t{ 0xef, 0xf1, 0xea, 0xbb, 0xb0, 0xed, 0xf5, 0xb4, 0xe8, 0xf4, 0xe8, 0xf4, 0xe6, 0xb7, 0xf6, 0xb0, },
    mask_t{ 0xe8, 0xf6, 0xed, 0xbc, 0xb7, 0xea, 0xf2, 0xb3, 0xef, 0xf3, 0xef, 0xf3, 0xe1, 0xb0, 0xf1, 0xb7, },
    mask_t{ 0xe9, 0xf7, 0xec, 0xbd, 0xb6, 0xeb, 0xf3, 0xb2, 0xee, 0xf2, 0xee, 0xf2, 0xe0, 0xb1, 0xf0, 0xb6, },
    mask_t{ 0xea, 0xf4, 0xef, 0xbe, 0xb5, 0xe8, 0xf0, 0xb1, 0xed, 0xf1, 0xed, 0xf1, 0xe3, 0xb2, 0xf3, 0xb5, },
    mask_t{ 0xeb, 0xf5, 0xee, 0xbf, 0xb4, 0xe9, 0xf1, 0xb0, 0xec, 0xf0, 0xec, 0xf0, 0xe2, 0xb3, 0xf2, 0xb4, },
    mask_t{ 0xe4, 0xfa, 0xe1, 0xb0, 0xbb, 0xe6, 0xfe, 0xbf, 0xe3, 0xff, 0xe3, 0xff, 0xed, 0xbc, 0xfd, 0xbb, },
    mask_t{ 0xe5, 0xfb, 0xe0, 0xb1, 0xba, 0xe7, 0xff, 0xbe, 0xe2, 0xfe, 0xe2, 0xfe, 0xec, 0xbd, 0xfc, 0xba, },
    mask_t{ 0xe6, 0xf8, 0xe3, 0xb2, 0xb9, 0xe4, 0xfc, 0xbd, 0xe1, 0xfd, 0xe1, 0xfd, 0xef, 0xbe, 0xff, 0xb9, },
    mask_t{ 0xe7, 0xf9, 0xe2, 0xb3, 0xb8, 0xe5, 0xfd, 0xbc, 0xe0, 0xfc, 0xe0, 0xfc, 0xee, 0xbf, 0xfe, 0xb8, },
    mask_t{ 0xe0, 0xfe, 0xe5, 0xb4, 0xbf, 0xe2, 0xfa, 0xbb, 0xe7, 0xfb, 0xe7, 0xfb, 0xe9, 0xb8, 0xf9, 0xbf, },
    mask_t{ 0xe1, 0xff, 0xe4, 0xb5, 0xbe, 0xe3, 0xfb, 0xba, 0xe6, 0xfa, 0xe6, 0xfa, 0xe8, 0xb9, 0xf8, 0xbe, },
    mask_t{ 0xe2, 0xfc, 0xe7, 0xb6, 0xbd, 0xe0, 0xf8, 0xb9, 0xe5, 0xf9, 0xe5, 0xf9, 0xeb, 0xba, 0xfb, 0xbd, },
    mask_t{ 0xe3, 0xfd, 0xe6, 0xb7, 0xbc, 0xe1, 0xf9, 0xb8, 0xe4, 0xf8, 0xe4, 0xf8, 0xea, 0xbb, 0xfa, 0xbc, },
    mask_t{ 0xfc, 0xe2, 0xf9, 0xa8, 0xa3, 0xfe, 0xe6, 0xa7, 0xfb, 0xe7, 0xfb, 0xe7, 0xf5, 0xa4, 0xe5, 0xa3, },
    mask_t{ 0xfd, 0xe3, 0xf8, 0xa9, 0xa2, 0xff, 0xe7, 0xa6, 0xfa, 0xe6, 0xfa, 0xe6, 0xf4, 0xa5, 0xe4, 0xa2, },
    mask_t{ 0xfe, 0xe0, 0xfb, 0xaa, 0xa1, 0xfc, 0xe4, 0xa5, 0xf9, 0xe5, 0xf9, 0xe5, 0xf7, 0xa6, 0xe7, 0xa1, },
    mask_t{ 0xff, 0xe1, 0xfa, 0xab, 0xa0, 0xfd, 0xe5, 0xa4, 0xf8, 0xe4, 0xf8, 0xe4, 0xf6, 0xa7, 0xe6, 0xa0, },
    mask_t{ 0xf8, 0xe6, 0xfd, 0xac, 0xa7, 0xfa, 0xe2, 0xa3, 0xff, 0xe3, 0xff, 0xe3, 0xf1, 0xa0, 0xe1, 0xa7, },
    mask_t{ 0xf9, 0xe7, 0xfc, 0xad, 0xa6, 0xfb, 0xe3, 0xa2, 0xfe, 0xe2, 0xfe, 0xe2, 0xf0, 0xa1, 0xe0, 0xa6, },
    mask_t{ 0xfa, 0xe4, 0xff, 0xae, 0xa5, 0xf8, 0xe0, 0xa1, 0xfd, 0xe1, 0xfd, 0xe1, 0xf3, 0xa2, 0xe3, 0xa5, },
    mask_t{ 0xfb, 0xe5, 0xfe, 0xaf, 0xa4, 0xf9, 0xe1, 0xa0, 0xfc, 0xe0, 0xfc, 0xe0, 0xf2, 0xa3, 0xe2, 0xa4, },
    mask_t{ 0xf4, 0xea, 0xf1, 0xa0, 0xab, 0xf6, 0xee, 0xaf, 0xf3, 0xef, 0xf3, 0xef, 0xfd, 0xac, 0xed, 0xab, },
    mask_t{ 0xf5, 0xeb, 0xf0, 0xa1, 0xaa, 0xf7, 0xef, 0xae, 0xf2, 0xee, 0xf2, 0xee, 0xfc, 0xad, 0xec, 0xaa, },
    mask_t{ 0xf6, 0xe8, 0xf3, 0xa2, 0xa9, 0xf4, 0xec, 0xad, 0xf1, 0xed, 0xf1, 0xed, 0xff, 0xae, 0xef, 0xa9, },
    mask_t{ 0xf7, 0xe9, 0xf2, 0xa3, 0xa8, 0xf5, 0xed, 0xac, 0xf0, 0xec, 0xf0, 0xec, 0xfe, 0xaf, 0xee, 0xa8, },
    mask_t{ 0xf0, 0xee, 0xf5, 0xa4, 0xaf, 0xf2, 0xea, 0xab, 0xf7, 0xeb, 0xf7, 0xeb, 0xf9, 0xa8, 0xe9, 0xaf, },
    mask_t{ 0xf1, 0xef, 0xf4, 0xa5, 0xae, 0xf3, 0xeb, 0xaa, 0xf6, 0xea, 0xf6, 0xea, 0xf8, 0xa9, 0xe8, 0xae, },
    mask_t{ 0xf2, 0xec, 0xf7, 0xa6, 0xad, 0xf0, 0xe8, 0xa9, 0xf5, 0xe9, 0xf5, 0xe9, 0xfb, 0xaa, 0xeb, 0xad, },
    mask_t{ 0xf3, 0xed, 0xf6, 0xa7, 0xac, 0xf1, 0xe9, 0xa8, 0xf4, 0xe8, 0xf4, 0xe8, 0xfa, 0xab, 0xea, 0xac, },
    mask_t{ 0xcc, 0xd2, 0xc9, 0x98, 0x93, 0xce, 0xd6, 0x97, 0xcb, 0xd7, 0xcb, 0xd7, 0xc5, 0x94, 0xd5, 0x93, },
    mask_t{ 0xcd, 0xd3, 0xc8, 0x99, 0x92, 0xcf, 0xd7, 0x96, 0xca, 0xd6, 0xca, 0xd6, 0xc4, 0x95, 0xd4, 0x92, },
    mask_t{ 0xce, 0xd0, 0xcb, 0x9a, 0x91, 0xcc, 0xd4, 0x95, 0xc9, 0xd5, 0xc9, 0xd5, 0xc7, 0x96, 0xd7, 0x91, },
    mask_t{ 0xcf, 0xd1, 0xca, 0x9b, 0x90, 0xcd, 0xd5, 0x94, 0xc8, 0xd4, 0xc8, 0xd4, 0xc6, 0x97, 0xd6, 0x90, },
    mask_t{ 0xc8, 0xd6, 0xcd, 0x9c, 0x97, 0xca, 0xd2, 0x93, 0xcf, 0xd3, 0xcf, 0xd3, 0xc1, 0x90, 0xd1, 0x97, },
    mask_t{ 0xc9, 0xd7, 0xcc, 0x9d, 0x96, 0xcb, 0xd3, 0x92, 0xce, 0xd2, 0xce, 0xd2, 0xc0, 0x91, 0xd0, 0x96, },
    mask_t{ 0xca, 0xd4, 0xcf, 0x9e, 0x95, 0xc8, 0xd0, 0x91, 0xcd, 0xd1, 0xcd, 0xd1, 0xc3, 0x92, 0xd3, 0x95, },
    mask_t{ 0xcb, 0xd5, 0xce, 0x9f, 0x94, 0xc9, 0xd1, 0x90, 0xcc, 0xd0, 0xcc, 0xd0, 0xc2, 0x93, 0xd2, 0x94, },
    mask_t{ 0xc4, 0xda, 0xc1, 0x90, 0x9b, 0xc6, 0xde, 0x9f, 0xc3, 0xdf, 0xc3, 0xdf, 0xcd, 0x9c, 0xdd, 0x9b, },
    mask_t{ 0xc5, 0xdb, 0xc0, 0x91, 0x9a, 0xc7, 0xdf, 0x9e, 0xc2, 0xde, 0xc2, 0xde, 0xcc, 0x9d, 0xdc, 0x9a, },
    mask_t{ 0xc6, 0xd8, 0xc3, 0x92, 0x99, 0xc4, 0xdc, 0x9d, 0xc1, 0xdd, 0xc1, 0xdd, 0xcf, 0x9e, 0xdf, 0x99, },
    mask_t{ 0xc7, 0xd9, 0xc2, 0x93, 0x98, 0xc5, 0xdd, 0x9c, 0xc0, 0xdc, 0xc0, 0xdc, 0xce, 0x9f, 0xde, 0x98, },
    mask_t{ 0xc0, 0xde, 0xc5, 0x94, 0x9f, 0xc2, 0xda, 0x9b, 0xc7, 0xdb, 0xc7, 0xdb, 0xc9, 0x98, 0xd9, 0x9f, },
    mask_t{ 0xc1, 0xdf, 0xc4, 0x95, 0x9e, 0xc3, 0xdb, 0x9a, 0xc6, 0xda, 0xc6, 0xda, 0xc8, 0x99, 0xd8, 0x9e, },
    mask_t{ 0xc2, 0xdc, 0xc7, 0x96, 0x9d, 0xc0, 0xd8, 0x99, 0xc5, 0xd9, 0xc5, 0xd9, 0xcb, 0x9a, 0xdb, 0x9d, },
    mask_t{ 0xc3, 0xdd, 0xc6, 0x97, 0x9c, 0xc1, 0xd9, 0x98, 0xc4, 0xd8, 0xc4, 0xd8, 0xca, 0x9b, 0xda, 0x9c, },
    mask_t{ 0xdc, 0xc2, 0xd9, 0x88, 0x83, 0xde, 0xc6, 0x87, 0xdb, 0xc7, 0xdb, 0xc7, 0xd5, 0x84, 0xc5, 0x83, },
    mask_t{ 0xdd, 0xc3, 0xd8, 0x89, 0x82, 0xdf, 0xc7, 0x86, 0xda, 0xc6, 0xda, 0xc6, 0xd4, 0x85, 0xc4, 0x82, },
    mask_t{ 0xde, 0xc0, 0xdb, 0x8a, 0x81, 0xdc, 0xc4, 0x85, 0xd9, 0xc5, 0xd9, 0xc5, 0xd7, 0x86, 0xc7, 0x81, },
    mask_t{ 0xdf, 0xc1, 0xda, 0x8b, 0x80, 0xdd, 0xc5, 0x84, 0xd8, 0xc4, 0xd8, 0xc4, 0xd6, 0x87, 0xc6, 0x80, },
    mask_t{ 0xd8, 0xc6, 0xdd, 0x8c, 0x87, 0xda, 0xc2, 0x83, 0xdf, 0xc3, 0xdf, 0xc3, 0xd1, 0x80, 0xc1, 0x87, },
    mask_t{ 0xd9, 0xc7, 0xdc, 0x8d, 0x86, 0xdb, 0xc3, 0x82, 0xde, 0xc2, 0xde, 0xc2, 0xd0, 0x81, 0xc0, 0x86, },
    mask_t{ 0xda, 0xc4, 0xdf, 0x8e, 0x85, 0xd8, 0xc0, 0x81, 0xdd, 0xc1, 0xdd, 0xc1, 0xd3, 0x82, 0xc3, 0x85, },
    mask_t{ 0xdb, 0xc5, 0xde, 0x8f, 0x84, 0xd9, 0xc1, 0x80, 0xdc, 0xc0, 0xdc, 0xc0, 0xd2, 0x83, 0xc2, 0x84, },
    mask_t{ 0xd4, 0xca, 0xd1, 0x80, 0x8b, 0xd6, 0xce, 0x8f, 0xd3, 0xcf, 0xd3, 0xcf, 0xdd, 0x8c, 0xcd, 0x8b, },
    mask_t{ 0xd5, 0xcb, 0xd0, 0x81, 0x8a, 0xd7, 0xcf, 0x8e, 0xd2, 0xce, 0xd2, 0xce, 0xdc, 0x8d, 0xcc, 0x8a, },
    mask_t{ 0xd6, 0xc8, 0xd3, 0x82, 0x89, 0xd4, 0xcc, 0x8d, 0xd1, 0xcd, 0xd1, 0xcd, 0xdf, 0x8e, 0xcf, 0x89, },
    mask_t{ 0xd7, 0xc9, 0xd2, 0x83, 0x88, 0xd5, 0xcd, 0x8c, 0xd0, 0xcc, 0xd0, 0xcc, 0xde, 0x8f, 0xce, 0x88, },
    mask_t{ 0xd0, 0xce, 0xd5, 0x84, 0x8f, 0xd2, 0xca, 0x8b, 0xd7, 0xcb, 0xd7, 0xcb, 0xd9, 0x88, 0xc9, 0x8f, },
    mask_t{ 0xd1, 0xcf, 0xd4, 0x85, 0x8e, 0xd3, 0xcb, 0x8a, 0xd6, 0xca, 0xd6, 0xca, 0xd8, 0x89, 0xc8, 0x8e, },
    mask_t{ 0xd2, 0xcc, 0xd7, 0x86, 0x8d, 0xd0, 0xc8, 0x89, 0xd5, 0xc9, 0xd5, 0xc9, 0xdb, 0x8a, 0xcb, 0x8d, },
    mask_t{ 0xd3, 0xcd, 0xd6, 0x87, 0x8c, 0xd1, 0xc9, 0x88, 0xd4, 0xc8, 0xd4, 0xc8, 0xda, 0x8b, 0xca, 0x8c, },
    mask_t{ 0xac, 0xb2, 0xa9, 0xf8, 0xf3, 0xae, 0xb6, 0xf7, 0xab, 0xb7, 0xab, 0xb7, 0xa5, 0xf4, 0xb5, 0xf3, },
    mask_t{ 0xad, 0xb3, 0xa8, 0xf9, 0xf2, 0xaf, 0xb7, 0xf6, 0xaa, 0xb6, 0xaa, 0xb6, 0xa4, 0xf5, 0xb4, 0xf2, },
    mask_t{ 0xae, 0xb0, 0xab, 0xfa, 0xf1, 0xac, 0xb4, 0xf5, 0xa9, 0xb5, 0xa9, 0xb5, 0xa7, 0xf6, 0xb7, 0xf1, },
    mask_t{ 0xaf, 0xb1, 0xaa, 0xfb, 0xf0, 0xad, 0xb5, 0xf4, 0xa8, 0xb4, 0xa8, 0xb4, 0xa6, 0xf7, 0xb6, 0xf0, },
    mask_t{ 0xa8, 0xb6, 0xad, 0xfc, 0xf7, 0xaa, 0xb2, 0xf3, 0xaf, 0xb3, 0xaf, 0xb3, 0xa1, 0xf0, 0xb1, 0xf7, },
    mask_t{ 0xa9, 0xb7, 0xac, 0xfd, 0xf6, 0xab, 0xb3, 0xf2, 0xae, 0xb2, 0xae, 0xb2, 0xa0, 0xf1, 0xb0, 0xf6, },
    mask_t{ 0xaa, 0xb4, 0xaf, 0xfe, 0xf5, 0xa8, 0xb0, 0xf1, 0xad, 0xb1, 0xad, 0xb1, 0xa3, 0xf2, 0xb3, 0xf5, },
    mask_t{ 0xab, 0xb5, 0xae, 0xff, 0xf4, 0xa9, 0xb1, 0xf0, 0xac, 0xb0, 0xac, 0xb0, 0xa2, 0xf3, 0xb2, 0xf4, },
    mask_t{ 0xa4, 0xba, 0xa1, 0xf0, 0xfb, 0xa6, 0xbe, 0xff, 0xa3, 0xbf, 0xa3, 0xbf, 0xad, 0xfc, 0xbd, 0xfb, },
    mask_t{ 0xa5, 0xbb, 0xa0, 0xf1, 0xfa, 0xa7, 0xbf, 0xfe, 0xa2, 0xbe, 0xa2, 0xbe, 0xac, 0xfd, 0xbc, 0xfa, },
    mask_t{ 0xa6, 0xb8, 0xa3, 0xf2, 0xf9, 0xa4, 0xbc, 0xfd, 0xa1, 0xbd, 0xa1, 0xbd, 0xaf, 0xfe, 0xbf, 0xf9, },
    mask_t{ 0xa7, 0xb9, 0xa2, 0xf3, 0xf8, 0xa5, 0xbd, 0xfc, 0xa0, 0xbc, 0xa0, 0xbc, 0xae, 0xff, 0xbe, 0xf8, },
    mask_t{ 0xa0, 0xbe, 0xa5, 0xf4, 0xff, 0xa2, 0xba, 0xfb, 0xa7, 0xbb, 0xa7, 0xbb, 0xa9, 0xf8, 0xb9, 0xff, },
    mask_t{ 0xa1, 0xbf, 0xa4, 0xf5, 0xfe, 0xa3, 0xbb, 0xfa, 0xa6, 0xba, 0xa6, 0xba, 0xa8, 0xf9, 0xb8, 0xfe, },
    mask_t{ 0xa2, 0xbc, 0xa7, 0xf6, 0xfd, 0xa0, 0xb8, 0xf9, 0xa5, 0xb9, 0xa5, 0xb9, 0xab, 0xfa, 0xbb, 0xfd, },
    mask_t{ 0xa3, 0xbd, 0xa6, 0xf7, 0xfc, 0xa1, 0xb9, 0xf8, 0xa4, 0xb8, 0xa4, 0xb8, 0xaa, 0xfb, 0xba, 0xfc, },
    mask_t{ 0xbc, 0xa2, 0xb9, 0xe8, 0xe3, 0xbe, 0xa6, 0xe7, 0xbb, 0xa7, 0xbb, 0xa7, 0xb5, 0xe4, 0xa5, 0xe3, },
    mask_t{ 0xbd, 0xa3, 0xb8, 0xe9, 0xe2, 0xbf, 0xa7, 0xe6, 0xba, 0xa6, 0xba, 0xa6, 0xb4, 0xe5, 0xa4, 0xe2, },
    mask_t{ 0xbe, 0xa0, 0xbb, 0xea, 0xe1, 0xbc, 0xa4, 0xe5, 0xb9, 0xa5, 0xb9, 0xa5, 0xb7, 0xe6, 0xa7, 0xe1, },
    mask_t{ 0xbf, 0xa1, 0xba, 0xeb, 0xe0, 0xbd, 0xa5, 0xe4, 0xb8, 0xa4, 0xb8, 0xa4, 0xb6, 0xe7, 0xa6, 0xe0, },
    mask_t{ 0xb8, 0xa6, 0xbd, 0xec, 0xe7, 0xba, 0xa2, 0xe3, 0xbf, 0xa3, 0xbf, 0xa3, 0xb1, 0xe0, 0xa1, 0xe7, },
    mask_t{ 0xb9, 0xa7, 0xbc, 0xed, 0xe6, 0xbb, 0xa3, 0xe2, 0xbe, 0xa2, 0xbe, 0xa2, 0xb0, 0xe1, 0xa0, 0xe6, },
    mask_t{ 0xba, 0xa4, 0xbf, 0xee, 0xe5, 0xb8, 0xa0, 0xe1, 0xbd, 0xa1, 0xbd, 0xa1, 0xb3, 0xe2, 0xa3, 0xe5, },
    mask_t{ 0xbb, 0xa5, 0xbe, 0xef, 0xe4, 0xb9, 0xa1, 0xe0, 0xbc, 0xa0, 0xbc, 0xa0, 0xb2, 0xe3, 0xa2, 0xe4, },
    mask_t{ 0xb4, 0xaa, 0xb1, 0xe0, 0xeb, 0xb6, 0xae, 0xef, 0xb3, 0xaf, 0xb3, 0xaf, 0xbd, 0xec, 0xad, 0xeb, },
    mask_t{ 0xb5, 0xab, 0xb0, 0xe1, 0xea, 0xb7, 0xaf, 0xee, 0xb2, 0xae, 0xb2, 0xae, 0xbc, 0xed, 0xac, 0xea, },
    mask_t{ 0xb6, 0xa8, 0xb3, 0xe2, 0xe9, 0xb4, 0xac, 0xed, 0xb1, 0xad, 0xb1, 0xad, 0xbf, 0xee, 0xaf, 0xe9, },
    mask_t{ 0xb7, 0xa9, 0xb2, 0xe3, 0xe8, 0xb5, 0xad, 0xec, 0xb0, 0xac, 0xb0, 0xac, 0xbe, 0xef, 0xae, 0xe8, },
    mask_t{ 0xb0, 0xae, 0xb5, 0xe4, 0xef, 0xb2, 0xaa, 0xeb, 0xb7, 0xab, 0xb7, 0xab, 0xb9, 0xe8, 0xa9, 0xef, },
    mask_t{ 0xb1, 0xaf, 0xb4, 0xe5, 0xee, 0xb3, 0xab, 0xea, 0xb6, 0xaa, 0xb6, 0xaa, 0xb8, 0xe9, 0xa8, 0xee, },
    mask_t{ 0xb2, 0xac, 0xb7, 0xe6, 0xed, 0xb0, 0xa8, 0xe9, 0xb5, 0xa9, 0xb5, 0xa9, 0xbb, 0xea, 0xab, 0xed, },
    mask_t{ 0xb3, 0xad, 0xb6, 0xe7, 0xec, 0xb1, 0xa9, 0xe8, 0xb4, 0xa8, 0xb4, 0xa8, 0xba, 0xeb, 0xaa, 0xec, },
    mask_t{ 0x8c, 0x92, 0x89, 0xd8, 0xd3, 0x8e, 0x96, 0xd7, 0x8b, 0x97, 0x8b, 0x97, 0x85, 0xd4, 0x95, 0xd3, },
    mask_t{ 0x8d, 0x93, 0x88, 0xd9, 0xd2, 0x8f, 0x97, 0xd6, 0x8a, 0x96, 0x8a, 0x96, 0x84, 0xd5, 0x94, 0xd2, },
    mask_t{ 0x8e, 0x90, 0x8b, 0xda, 0xd1, 0x8c, 0x94, 0xd5, 0x89, 0x95, 0x89, 0x95, 0x87, 0xd6, 0x97, 0xd1, },
    mask_t{ 0x8f, 0x91, 0x8a, 0xdb, 0xd0, 0x8d, 0x95, 0xd4, 0x88, 0x94, 0x88, 0x94, 0x86, 0xd7, 0x96, 0xd0, },
    mask_t{ 0x88, 0x96, 0x8d, 0xdc, 0xd7, 0x8a, 0x92, 0xd3, 0x8f, 0x93, 0x8f, 0x93, 0x81, 0xd0, 0x91, 0xd7, },
    mask_t{ 0x89, 0x97, 0x8c, 0xdd, 0xd6, 0x8b, 0x93, 0xd2, 0x8e, 0x92, 0x8e, 0x92, 0x80, 0xd1, 0x90, 0xd6, },
    mask_t{ 0x8a, 0x94, 0x8f, 0xde, 0xd5, 0x88, 0x90, 0xd1, 0x8d, 0x91, 0x8d, 0x91, 0x83, 0xd2, 0x93, 0xd5, },
    mask_t{ 0x8b, 0x95, 0x8e, 0xdf, 0xd4, 0x89, 0x91, 0xd0, 0x8c, 0x90, 0x8c, 0x90, 0x82, 0xd3, 0x92, 0xd4, },
    mask_t{ 0x84, 0x9a, 0x81, 0xd0, 0xdb, 0x86, 0x9e, 0xdf, 0x83, 0x9f, 0x83, 0x9f, 0x8d, 0xdc, 0x9d, 0xdb, },
    mask_t{ 0x85, 0x9b, 0x80, 0xd1, 0xda, 0x87, 0x9f, 0xde, 0x82, 0x9e, 0x82, 0x9e, 0x8c, 0xdd, 0x9c, 0xda, },
    mask_t{ 0x86, 0x98, 0x83, 0xd2, 0xd9, 0x84, 0x9c, 0xdd, 0x81, 0x9d, 0x81, 0x9d, 0x8f, 0xde, 0x9f, 0xd9, },
    mask_t{ 0x87, 0x99, 0x82, 0xd3, 0xd8, 0x85, 0x9d, 0xdc, 0x80, 0x9c, 0x80, 0x9c, 0x8e, 0xdf, 0x9e, 0xd8, },
    mask_t{ 0x80, 0x9e, 0x85, 0xd4, 0xdf, 0x82, 0x9a, 0xdb, 0x87, 0x9b, 0x87, 0x9b, 0x89, 0xd8, 0x99, 0xdf, },
    mask_t{ 0x81, 0x9f, 0x84, 0xd5, 0xde, 0x83, 0x9b, 0xda, 0x86, 0x9a, 0x86, 0x9a, 0x88, 0xd9, 0x98, 0xde, },
    mask_t{ 0x82, 0x9c, 0x87, 0xd6, 0xdd, 0x80, 0x98, 0xd9, 0x85, 0x99, 0x85, 0x99, 0x8b, 0xda, 0x9b, 0xdd, },
    mask_t{ 0x83, 0x9d, 0x86, 0xd7, 0xdc, 0x81, 0x99, 0xd8, 0x84, 0x98, 0x84, 0x98, 0x8a, 0xdb, 0x9a, 0xdc, },
    mask_t{ 0x9c, 0x82, 0x99, 0xc8, 0xc3, 0x9e, 0x86, 0xc7, 0x9b, 0x87, 0x9b, 0x87, 0x95, 0xc4, 0x85, 0xc3, },
    mask_t{ 0x9d, 0x83, 0x98, 0xc9, 0xc2, 0x9f, 0x87, 0xc6, 0x9a, 0x86, 0x9a, 0x86, 0x94, 0xc5, 0x84, 0xc2, },
    mask_t{ 0x9e, 0x80, 0x9b, 0xca, 0xc1, 0x9c, 0x84, 0xc5, 0x99, 0x85, 0x99, 0x85, 0x97, 0xc6, 0x87, 0xc1, },
    mask_t{ 0x9f, 0x81, 0x9a, 0xcb, 0xc0, 0x9d, 0x85, 0xc4, 0x98, 0x84, 0x98, 0x84, 0x96, 0xc7, 0x86, 0xc0, },
    mask_t{ 0x98, 0x86, 0x9d, 0xcc, 0xc7, 0x9a, 0x82, 0xc3, 0x9f, 0x83, 0x9f, 0x83, 0x91, 0xc0, 0x81, 0xc7, },
    mask_t{ 0x99, 0x87, 0x9c, 0xcd, 0xc6, 0x9b, 0x83, 0xc2, 0x9e, 0x82, 0x9e, 0x82, 0x90, 0xc1, 0x80, 0xc6, },
    mask_t{ 0x9a, 0x84, 0x9f, 0xce, 0xc5, 0x98, 0x80, 0xc1, 0x9d, 0x81, 0x9d, 0x81, 0x93, 0xc2, 0x83, 0xc5, },
    mask_t{ 0x9b, 0x85, 0x9e, 0xcf, 0xc4, 0x99, 0x81, 0xc0, 0x9c, 0x80, 0x9c, 0x80, 0x92, 0xc3, 0x82, 0xc4, },
    mask_t{ 0x94, 0x8a, 0x91, 0xc0, 0xcb, 0x96, 0x8e, 0xcf, 0x93, 0x8f, 0x93, 0x8f, 0x9d, 0xcc, 0x8d, 0xcb, },
    mask_t{ 0x95, 0x8b, 0x90, 0xc1, 0xca, 0x97, 0x8f, 0xce, 0x92, 0x8e, 0x92, 0x8e, 0x9c, 0xcd, 0x8c, 0xca, },
    mask_t{ 0x96, 0x88, 0x93, 0xc2, 0xc9, 0x94, 0x8c, 0xcd, 0x91, 0x8d, 0x91, 0x8d, 0x9f, 0xce, 0x8f, 0xc9, },
    mask_t{ 0x97, 0x89, 0x92, 0xc3, 0xc8, 0x95, 0x8d, 0xcc, 0x90, 0x8c, 0x90, 0x8c, 0x9e, 0xcf, 0x8e, 0xc8, },
    mask_t{ 0x90, 0x8e, 0x95, 0xc4, 0xcf, 0x92, 0x8a, 0xcb, 0x97, 0x8b, 0x97, 0x8b, 0x99, 0xc8, 0x89, 0xcf, },
    mask_t{ 0x91, 0x8f, 0x94, 0xc5, 0xce, 0x93, 0x8b, 0xca, 0x96, 0x8a, 0x96, 0x8a, 0x98, 0xc9, 0x88, 0xce, },
    mask_t{ 0x92, 0x8c, 0x97, 0xc6, 0xcd, 0x90, 0x88, 0xc9, 0x95, 0x89, 0x95, 0x89, 0x9b, 0xca, 0x8b, 0xcd, },
    mask_t{ 0x93, 0x8d, 0x96, 0xc7, 0xcc, 0x91, 0x89, 0xc8, 0x94, 0x88, 0x94, 0x88, 0x9a, 0xcb, 0x8a, 0xcc, },
};


int main(int argc, char* argv[]) try
{
    CommandLineParameters parameters = process_command_line_parameters(argc, argv);
    std::vector<byte_t> data = read_from_file(parameters.input_filename);
    process_data(data, DECRYPTION_TABLE);
    store_to_file(parameters.output_filename, data);
    std::cout << "File decrypted!\n";
}
catch (const std::exception& e)
{
    std::cerr << e.what() << "\nAborting\n";
    return 1;
}
