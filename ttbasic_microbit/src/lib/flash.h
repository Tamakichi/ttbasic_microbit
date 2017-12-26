//
// �t���b�V���������������݃��C�u����
// 2017/12/25 by ���܋g����
// �Q�l����
//  nRF51 Series Reference Manual Version 3.0 �u6 Non-Volatile Memory Controller (NVMC)�v
//

#ifndef __FLASH_H__
#define __FLASH_H__

#include <Arduino.h>

// ����
//   addr �����Ώۃy�[�W�擪�A�h���X
// �߂�l
//   0:����I��
//   1:�ُ�I��(�w��A�h���X���������Ȃ�)
//
uint8_t  flashPageErase(uint32_t addr);

// �t���b�V��������32�r�b�g���[�h�f�[�^��������
// ����
//  addr: �����݃A�h���X(4�o�C�g���E�A�h���X�ł��邱��)
//  ptr : �����݃f�[�^�i�[�A�h���X(4�o�C�g���E�A�h���X�ł��邱��)
//  len : �����݃f�[�^����
// �߂�l
//   0:����I��
//   1:�ُ�I��(�w��A�h���X���������Ȃ�)
//
uint8_t flashWriteData(uint32_t* addr, uint32_t *ptr, uint16_t len);

#endif
