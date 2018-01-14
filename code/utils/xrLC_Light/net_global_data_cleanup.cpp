#include "stdafx.h"

#include "net_global_data_cleanup.h"
#include "serialize.h"
namespace lc_net {
global_data_cleanup _cleanup;
global_data_cleanup& cleanup() { return _cleanup; }

global_data_cleanup::global_data_cleanup() : id_state(0) { vec_cleanup.resize(gl_last, 0); }

void global_data_cleanup::on_net_send(IGenericStream* outStream) {
    outStream->Write(&id_state, sizeof(id_state));
}

static const u32 data_cleanup_callback_read_write_buff_size = 512;

void __cdecl data_cleanup_callback(const char* dataDesc, IGenericStream** stream) {
    *stream = CreateGenericStream();
    u8 buff[data_cleanup_callback_read_write_buff_size];
    INetMemoryBuffWriter netBlockReader(*stream, sizeof(buff), buff);
    w_pod_vector(netBlockReader, _cleanup.vec_cleanup);
    // w_pod_vector( INetIWriterGenStream( *stream, 512 ), _cleanup.vec_cleanup );
}

#pragma warning(disable : 4995)
void global_data_cleanup::on_net_receive(IAgent* agent, DWORD sessionId, IGenericStream* inStream) {
    u32 state = u32(-1);
    inStream->Read(&state, sizeof(id_state));
    std::lock_guard<decltype(lock)> locker(lock);
    if (state == id_state)
        return;

    IGenericStream* globalDataStream = nullptr;
    HRESULT rz = agent->GetData(sessionId, "data_cleanup", &globalDataStream);
    if (rz != S_OK)
        return;

    R_ASSERT(globalDataStream);
    u8 buff[data_cleanup_callback_read_write_buff_size];
    INetBlockReader netBlockReader(globalDataStream, buff, sizeof(buff));
    r_pod_vector(netBlockReader, vec_cleanup);
    //  r_pod_vector( INetReaderGenStream( globalDataStream ), vec_cleanup );

    free(globalDataStream->GetCurPointer());
    id_state = state;
#ifdef CL_NET_LOG
    Msg("cleanup call");
#endif
    globals().cleanup();
}
#pragma warning(default : 4995)
} // namespace lc_net