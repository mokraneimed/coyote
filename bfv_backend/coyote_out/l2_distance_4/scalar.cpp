
# include <scalar.hpp>
int ScalarProgram::num_registers()
{
    return 23;
}

std::vector<std::string> ScalarProgram::vars_used()
{
    return {"l2_distance_4(b):3", "l2_distance_4(b):2", "l2_distance_4(a):2", "l2_distance_4(b):1", "l2_distance_4(a):1", "l2_distance_4(a):0", "l2_distance_4(a):3", "l2_distance_4(b):0"};
}

std::vector<ctxt> ScalarProgram::computation(std::map<std::string, ctxt> locs, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    ctxt regs[num_registers()];
    regs[0] = locs["l2_distance_4(a):0"];
    regs[1] = locs["l2_distance_4(b):0"];
    info.eval->sub(regs[0], regs[1], regs[2]);
    info.eval->sub(regs[0], regs[1], regs[3]);
    info.eval->multiply(regs[2], regs[3], regs[4]);
    info.eval->relinearize_inplace(regs[4], rk);
    regs[5] = locs["l2_distance_4(a):1"];
    regs[6] = locs["l2_distance_4(b):1"];
    info.eval->sub(regs[5], regs[6], regs[7]);
    info.eval->sub(regs[5], regs[6], regs[8]);
    info.eval->multiply(regs[7], regs[8], regs[9]);
    info.eval->relinearize_inplace(regs[9], rk);
    info.eval->add(regs[4], regs[9], regs[10]);
    regs[11] = locs["l2_distance_4(a):2"];
    regs[12] = locs["l2_distance_4(b):2"];
    info.eval->sub(regs[11], regs[12], regs[13]);
    info.eval->sub(regs[11], regs[12], regs[14]);
    info.eval->multiply(regs[13], regs[14], regs[15]);
    info.eval->relinearize_inplace(regs[15], rk);
    regs[16] = locs["l2_distance_4(a):3"];
    regs[17] = locs["l2_distance_4(b):3"];
    info.eval->sub(regs[16], regs[17], regs[18]);
    info.eval->sub(regs[16], regs[17], regs[19]);
    info.eval->multiply(regs[18], regs[19], regs[20]);
    info.eval->relinearize_inplace(regs[20], rk);
    info.eval->add(regs[15], regs[20], regs[21]);
    info.eval->add(regs[10], regs[21], regs[22]);
    std::vector<ctxt> answer;
    answer.push_back(regs[22]);
    return answer;
}
    