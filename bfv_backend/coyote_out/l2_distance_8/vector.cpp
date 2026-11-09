
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "000001100000000", info);
    add_bitstring(bits, "110001101100000", info);
    add_bitstring(bits, "000000000000011", info);
    add_bitstring(bits, "000000000000001", info);
    add_bitstring(bits, "000000100000000", info);
    add_bitstring(bits, "000000000000010", info);
    add_bitstring(bits, "100000001100000", info);
    add_bitstring(bits, "000001000000000", info);
    add_bitstring(bits, "100000001100011", info);
    add_bitstring(bits, "010000000000000", info);
    add_bitstring(bits, "001000010011000", info);
    add_bitstring(bits, "110001101100011", info);
    add_bitstring(bits, "001000010011011", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(12);
    ts[0] = encrypt_input("0011111111111111111011111111111111111100011111111111111111111111111111111111101111111111111111111111111111111111110111111111111111111111111111111111111", info);
    ts[1] = encrypt_input("1111111111111111101111111111111111110001111111111111111111111111111111111110111111111111111111111111111111111111000111111111111111111111111111111111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[11];
    ctxt ss[6];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -13, gk, ss[0]); // __s0 = __v0 >> 13
    vs[1] = ts[1];
    info.eval->rotate_rows(vs[1], -2, gk, ss[1]); // __s1 = __v1 >> 2
    
    // __t2 = blend(__s0@110001101100000, __v0@001000010011011)
    {
    ctxt t2_1, t2_2;
    info.eval->multiply_plain(ss[0], bits["110001101100000"], t2_1);
    info.eval->multiply_plain(vs[0], bits["001000010011011"], t2_2);
    info.eval->add(t2_1, t2_2, ts[2]);
    }
    
    
    // __t3 = blend(__v1@110001101100011, __s1@001000010011000)
    {
    ctxt t3_1, t3_2;
    info.eval->multiply_plain(vs[1], bits["110001101100011"], t3_1);
    info.eval->multiply_plain(ss[1], bits["001000010011000"], t3_2);
    info.eval->add(t3_1, t3_2, ts[3]);
    }
    
    info.eval->sub(ts[2], ts[3], vs[2]); // __v2 = __t2 - __t3
    info.eval->rotate_rows(vs[2], -13, gk, ss[2]); // __s2 = __v2 >> 13
    
    // __t4 = blend(__s0@010000000000000, __v0@000000000000011)
    {
    ctxt t4_1, t4_2;
    info.eval->multiply_plain(ss[0], bits["010000000000000"], t4_1);
    info.eval->multiply_plain(vs[0], bits["000000000000011"], t4_2);
    info.eval->add(t4_1, t4_2, ts[4]);
    }
    
    info.eval->sub(ts[4], vs[1], vs[3]); // __v3 = __t4 - __v1
    
    // __t5 = blend(__v2@100000001100011, __v3@010000000000000)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(vs[2], bits["100000001100011"], t5_1);
    info.eval->multiply_plain(vs[3], bits["010000000000000"], t5_2);
    info.eval->add(t5_1, t5_2, ts[5]);
    }
    
    
    // __t6 = blend(__s2@100000001100000, __v2@010000000000000, __v3@000000000000011)
    {
    ctxt t6_1, t6_2, t6_3;
    info.eval->multiply_plain(ss[2], bits["100000001100000"], t6_1);
    info.eval->multiply_plain(vs[2], bits["010000000000000"], t6_2);
    info.eval->multiply_plain(vs[3], bits["000000000000011"], t6_3);
    info.eval->add_many({t6_1, t6_2, t6_3}, ts[6]);
    }
    
    info.eval->multiply(ts[5], ts[6], vs[4]); // __v4 = __t5 * __t6
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -5, gk, ss[3]); // __s3 = __v4 >> 5
    info.eval->sub(ss[0], vs[1], vs[5]); // __v5 = __s0 - __v1
    
    // __t7 = blend(__s2@000001000000000, __v2@000000100000000)
    {
    ctxt t7_1, t7_2;
    info.eval->multiply_plain(ss[2], bits["000001000000000"], t7_1);
    info.eval->multiply_plain(vs[2], bits["000000100000000"], t7_2);
    info.eval->add(t7_1, t7_2, ts[7]);
    }
    
    
    // __t8 = blend(__v2@000001000000000, __v5@000000100000000)
    {
    ctxt t8_1, t8_2;
    info.eval->multiply_plain(vs[2], bits["000001000000000"], t8_1);
    info.eval->multiply_plain(vs[5], bits["000000100000000"], t8_2);
    info.eval->add(t8_1, t8_2, ts[8]);
    }
    
    info.eval->multiply(ts[7], ts[8], vs[6]); // __v6 = __t7 * __t8
    info.eval->relinearize_inplace(vs[6], rk);
    
    // __t9 = blend(__v6@000001100000000, __v4@000000000000001)
    {
    ctxt t9_1, t9_2;
    info.eval->multiply_plain(vs[6], bits["000001100000000"], t9_1);
    info.eval->multiply_plain(vs[4], bits["000000000000001"], t9_2);
    info.eval->add(t9_1, t9_2, ts[9]);
    }
    
    info.eval->add(ss[3], ts[9], vs[7]); // __v7 = __s3 + __t9
    info.eval->rotate_rows(vs[7], -14, gk, ss[4]); // __s4 = __v7 >> 14
    
    // __t10 = blend(__v7@000001000000000, __s3@000000000000010)
    {
    ctxt t10_1, t10_2;
    info.eval->multiply_plain(vs[7], bits["000001000000000"], t10_1);
    info.eval->multiply_plain(ss[3], bits["000000000000010"], t10_2);
    info.eval->add(t10_1, t10_2, ts[10]);
    }
    
    
    // __t11 = blend(__s4@000001000000000, __v4@000000000000010)
    {
    ctxt t11_1, t11_2;
    info.eval->multiply_plain(ss[4], bits["000001000000000"], t11_1);
    info.eval->multiply_plain(vs[4], bits["000000000000010"], t11_2);
    info.eval->add(t11_1, t11_2, ts[11]);
    }
    
    info.eval->add(ts[10], ts[11], vs[8]); // __v8 = __t10 + __t11
    info.eval->add(ss[4], vs[8], vs[9]); // __v9 = __s4 + __v8
    info.eval->rotate_rows(vs[9], -7, gk, ss[5]); // __s5 = __v9 >> 7
    info.eval->add(vs[8], ss[5], vs[10]); // __v10 = __v8 + __s5
    return vs[10];
}
    