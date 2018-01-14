#include "stdafx.h"
#include "dx11HDAOCSBlender.h"	// � HDAO ������� ������� ��������� �������
void CBlender_CS_HDAO::Compile(CBlender_Compile& C) { // ������� � HDAO
    IBlender::Compile(C);

    switch (C.iElement) {
    case 0:
		// ������� ���������� HDAO 
        C.r_ComputePass("ssao_hdao"); // HDAO ����������� � SSAO ������ ������ � ����������� //

        C.r_dx10Texture("s_position", r2_RT_P); // ������� ������������
        C.r_dx10Texture("s_normal", r2_RT_N); // ����� ��������

        C.r_dx10Sampler("smp_nofilter"); // � ����� ����������� ������ ���������

        C.r_End();

        break;
    }
}

void CBlender_CS_HDAO_MSAA::Compile(CBlender_Compile& C) {	// ������� � HDAO + MSAA
    IBlender::Compile(C);

    switch (C.iElement) {
    case 0:
        C.r_ComputePass("ssao_hdao_msaa"); // ���� �����, ������ SSAO ������ � MSAA //

        C.r_dx10Texture("s_position", r2_RT_P); 
        C.r_dx10Texture("s_normal", r2_RT_N);

        C.r_dx10Sampler("smp_nofilter");

        C.r_End();

        break;
    }
}
// ����������: MSAA � DX10 �������� � ����� ������, ������� ����� ���� ������������!