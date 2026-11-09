
# include <scalar.hpp>
int ScalarProgram::num_registers()
{
    return 47;
}

std::vector<std::string> ScalarProgram::vars_used()
{
    return {"l2_distance_8(b):0", "l2_distance_8(b):2", "l2_distance_8(a):3", "l2_distance_8(b):7", "l2_distance_8(a):2", "l2_distance_8(b):6", "l2_distance_8(a):0", "l2_distance_8(b):5", "l2_distance_8(a):4", "l2_distance_8(a):6", "l2_distance_8(a):7", "l2_distance_8(b):3", "l2_distance_8(b):1", "l2_distance_8(a):1", "l2_distance_8(a):5", "l2_distance_8(b):4"};
}

std::vector<ctxt> ScalarProgram::computation(std::map<std::string, ctxt> locs, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    ctxt regs[num_registers()];
    regs[0] = locs["l2_distance_8(a):0"];
    regs[1] = locs["l2_distance_8(b):0"];
    info.eval->sub(regs[0], regs[1], regs[2]);
    info.eval->sub(regs[0], regs[1], regs[3]);
    info.eval->multiply(regs[2], regs[3], regs[4]);
    info.eval->relinearize_inplace(regs[4], rk);
    regs[5] = locs["l2_distance_8(a):1"];
    regs[6] = locs["l2_distance_8(b):1"];
    info.eval->sub(regs[5], regs[6], regs[7]);
    info.eval->sub(regs[5], regs[6], regs[8]);
    info.eval->multiply(regs[7], regs[8], regs[9]);
    info.eval->relinearize_inplace(regs[9], rk);
    info.eval->add(regs[4], regs[9], regs[10]);
    regs[11] = locs["l2_distance_8(a):2"];
    regs[12] = locs["l2_distance_8(b):2"];
    info.eval->sub(regs[11], regs[12], regs[13]);
    info.eval->sub(regs[11], regs[12], regs[14]);
    info.eval->multiply(regs[13], regs[14], regs[15]);
    info.eval->relinearize_inplace(regs[15], rk);
    regs[16] = locs["l2_distance_8(a):3"];
    regs[17] = locs["l2_distance_8(b):3"];
    info.eval->sub(regs[16], regs[17], regs[18]);
    info.eval->sub(regs[16], regs[17], regs[19]);
    info.eval->multiply(regs[18], regs[19], regs[20]);
    info.eval->relinearize_inplace(regs[20], rk);
    info.eval->add(regs[15], regs[20], regs[21]);
    info.eval->add(regs[10], regs[21], regs[22]);
    regs[23] = locs["l2_distance_8(a):4"];
    regs[24] = locs["l2_distance_8(b):4"];
    info.eval->sub(regs[23], regs[24], regs[25]);
    info.eval->sub(regs[23], regs[24], regs[26]);
    info.eval->multiply(regs[25], regs[26], regs[27]);
    info.eval->relinearize_inplace(regs[27], rk);
    regs[28] = locs["l2_distance_8(a):5"];
    regs[29] = locs["l2_distance_8(b):5"];
    info.eval->sub(regs[28], regs[29], regs[30]);
    info.eval->sub(regs[28], regs[29], regs[31]);
    info.eval->multiply(regs[30], regs[31], regs[32]);
    info.eval->relinearize_inplace(regs[32], rk);
    info.eval->add(regs[27], regs[32], regs[33]);
    regs[34] = locs["l2_distance_8(a):6"];
    regs[35] = locs["l2_distance_8(b):6"];
    info.eval->sub(regs[34], regs[35], regs[36]);
    info.eval->sub(regs[34], regs[35], regs[37]);
    info.eval->multiply(regs[36], regs[37], regs[38]);
    info.eval->relinearize_inplace(regs[38], rk);
    regs[39] = locs["l2_distance_8(a):7"];
    regs[40] = locs["l2_distance_8(b):7"];
    info.eval->sub(regs[39], regs[40], regs[41]);
    info.eval->sub(regs[39], regs[40], regs[42]);
    info.eval->multiply(regs[41], regs[42], regs[43]);
    info.eval->relinearize_inplace(regs[43], rk);
    info.eval->add(regs[38], regs[43], regs[44]);
    info.eval->add(regs[33], regs[44], regs[45]);
    info.eval->add(regs[22], regs[45], regs[46]);
    std::vector<ctxt> answer;
    answer.push_back(regs[46]);
    return answer;
}
    