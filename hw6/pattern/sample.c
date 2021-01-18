int f1(){
    write("f1\n");
    return 1;
}

float f2(){
    write("f2\n");
    return 0.0;
}

int MAIN(){
    float a = 123.123;
    write(a);
    write("\n");
    a = f1();
    write(a);
    write("\n");

}


