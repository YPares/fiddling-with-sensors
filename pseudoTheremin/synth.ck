SinOsc s => Gain g => dac;
g.gain(0.8);

0 => int device;
SerialIO c;

if(!c.open(device, SerialIO.B9600, SerialIO.ASCII)) {
  chout <= "Unable to open serial device\n";
  me.exit();
 }

fun int clamp(int x, int a, int b) {
  if(x > b) return b;
  else if(x < a) return a;
  else return x;
}

while(true){
  c.onLine() => now;
  c.getLine() => string line;

  if(line$Object != null) {
    StringTokenizer tok;
    tok.set(line);
    Std.atoi(tok.next()) => int pos;
    clamp(pos+30, 45, 80) => int note;
    s.freq(Std.mtof(note));
  }
}   
