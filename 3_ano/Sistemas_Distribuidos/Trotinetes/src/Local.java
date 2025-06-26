import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

/* -----------------------------------------------------------------------------------------------*/
                                        /* Local */
/*Métodos--> norma(distância manhattan)*/

public class Local {
    /* Variáveis */
    public int x,y;
    /* Construtor */
    public Local(int x, int y){
        this.x = x;
        this.y = y;
    }

    @Override
    public String toString(){
        return "x=" + x + " y=" + y;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Local local = (Local) o;
        return x == local.x && y == local.y;
    }

    public Local clone(){
        return new Local(x,y);
    }

    public static int norma(Local one, Local two) {/* metodo para fazer a distância entre dois locais */
        return Math.abs(one.x - two.x) + Math.abs(one.y - two.y);
    }

    public void serialize (DataOutputStream out) throws IOException{
        out.writeInt(x);
        out.writeInt(y);
    }

    public static Local deserialize (DataInputStream in) throws IOException{
        return new Local(in.readInt(),in.readInt());
    }
}