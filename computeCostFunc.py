import networkx as nx
import math
def quant(l_e,to_fbits):
    scale=math.pow(2.0,to_fbits)
    return round(scale * l_e)/scale;
def isConst(l_e,l_v):
    if(l_v==0):
        return True
    else:
        return False

def q_c(l_e, l_v,l_ne,l_nv,l_cov,from_fbits, to_fbits):
    if(isConst(l_e, l_v)==False):
        if(from_fbits>to_fbits):
            qnoise_var = 1.0/3*math.pow(2.0,-2*to_fbits)
            if(qnoise_var > l_v):
                return 0;
    return l_cov;

def q_ne(l_e,l_v,l_ne,l_nv,l_cov,from_fbits, to_fbits):
    if(isConst(l_e, l_v)==False):
        if(from_fbits>to_fbits):
            qnoise_var = 1.0/3*math.pow(2.0,-2*to_fbits)
            if(qnoise_var > l_v):
                return 0;
    else:
        return quant(l_e,to_fbits)-l_e;
    return l_ne;

def q_nv(l_e,l_v,l_ne,l_nv,l_cov,from_fbits, to_fbits):
    qnoise_var=0;
    if(isConst(l_e, l_v)==False):
        if(from_fbits>to_fbits):
            qnoise_var = 1.0/3*math.pow(2.0,-2*to_fbits)
            print 'q_nv',qnoise_var,from_fbits,to_fbits
            if(qnoise_var > l_v):
                qnoise_var=l_v
    return qnoise_var+l_nv;



def ne_add(l_e,l_v,l_ne,l_nv,l_cov, r_e,r_v,r_ne,r_nv,r_cov, \
    l_from_fbits,r_from_fbits,to_fbits):
    ne_a=q_ne(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,to_fbits)
    ne_b=q_ne(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,to_fbits)
    return ne_a+ne_b;


def nv_add(l_e,l_v,l_ne,l_nv,l_cov, r_e,r_v,r_ne,r_nv,r_cov, \
    l_from_fbits,r_from_fbits,to_fbits):
    print "nv_a",l_from_fbits,r_from_fbits, to_fbits
    nv_a=q_nv(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,to_fbits)
    nv_b=q_nv(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,to_fbits)
    return nv_a+nv_b;


def c_add(l_e,l_v,l_ne,l_nv,l_cov, r_e,r_v,r_ne,r_nv,r_cov, \
    l_from_fbits,r_from_fbits,to_fbits):
    c_a=q_c(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,to_fbits)
    c_b=q_c(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,to_fbits)
    return c_a+c_b;

def ne_mul(l_e,l_v,l_ne,l_nv,l_cov, r_e,r_v,r_ne,r_nv,r_cov, \
    l_from_fbits, l_to_fbits, r_from_fbits, r_to_fbits):
    coeff=0;
    coeff_err=0;
    x_v=0;
    x_nv=0;
    x_c=0;
    if(isConst(l_e,l_v)==True and isConst(r_e,r_v)==False):
        coeff=l_e;
        coeff_err=q_ne(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,l_to_fbits);
        x_e=r_e;
        x_ne=q_ne(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,r_to_fbits)
        pass
    elif(isConst(l_e,l_v)==False and isConst(r_e,r_v)==True):
        coeff=r_e;
        coeff_err=q_ne(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,r_to_fbits);
        x_e=l_e;
        x_ne=q_ne(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,l_to_fbits);
    else:
        return "Error"
    ne=(coeff+coeff_err)*x_ne+(coeff_err)*x_e
    return ne;

def nv_mul(l_e,l_v,l_ne,l_nv,l_cov, r_e,r_v,r_ne,r_nv,r_cov, \
    l_from_fbits, l_to_fbits, r_from_fbits, r_to_fbits):
    coeff=0;
    coeff_err=0;
    x_v=0;
    x_nv=0;
    x_c=0;
    if(isConst(l_e,l_v)==True and isConst(r_e,r_v)==False):
        coeff=l_e;
        coeff_err=q_ne(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,l_to_fbits);
        x_v=r_v;
        x_nv=q_nv(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,r_to_fbits)
        x_c=q_c(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,r_to_fbits)
        pass
    elif(isConst(l_e,l_v)==False and isConst(r_e,r_v)==True):
        coeff=r_e;
        coeff_err=q_ne(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,r_to_fbits);
        x_v=l_v;
        x_nv=q_nv(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,l_to_fbits);
        x_c=q_c(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,l_to_fbits);
    else:
        return "Error"
    nv=(coeff+coeff_err)*(coeff+coeff_err)*x_nv+\
        (coeff_err)*(coeff_err)*x_v+\
        2*(coeff+coeff_err)*(coeff)*x_c;
    return nv;

def c_mul(l_e,l_v,l_ne,l_nv,l_cov, r_e,r_v,r_ne,r_nv,r_cov, \
    l_from_fbits, l_to_fbits, r_from_fbits, r_to_fbits):
    coeff=0;
    coeff_err=0;
    x_v=0;
    x_nv=0;
    x_c=0;
    if(isConst(l_e,l_v)==True and isConst(r_e,r_v)==False):
        coeff=l_e;
        coeff_err=q_ne(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,l_to_fbits);
        x_v=r_v;
        x_nv=q_nv(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,r_to_fbits)
        x_c=q_c(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,r_to_fbits)
        pass
    elif(isConst(l_e,l_v)==False and isConst(r_e,r_v)==True):
        coeff=r_e;
        coeff_err=q_ne(r_e,r_v,r_ne,r_nv,r_cov,r_from_fbits,r_to_fbits);
        x_v=l_v;
        x_nv=q_nv(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,l_to_fbits);
        x_c=q_c(l_e,l_v,l_ne,l_nv,l_cov,l_from_fbits,l_to_fbits);
    else:
        return "Error"
    c=(coeff+coeff_err)*(coeff_err)*x_v+\
        (coeff_err)*(coeff_err+coeff)*x_c
    return c;

def data_cost(graph,idx):
    plist=graph.predecessors(idx)
    if(len(plist)==1):
        if('0' in plist):
            graph.node[idx]['NE']=0
            graph.node[idx]['NV']=0
            graph.node[idx]['COV']=0
        else:
            graph.node[idx]['NE']=graph.node[plist[0]]['NE']
            graph.node[idx]['NV']=graph.node[plist[0]]['NV']
            graph.node[idx]['COV']=graph.node[plist[0]]['COV']
    else:
        print 'Error: not support yet'

def adder_cost(graph,idx):
    global node_width;
    plist=graph.predecessors(idx);
    print plist
    if(len(plist)==2):
        args=(
            graph.node[plist[0]]['E']  ,
            graph.node[plist[0]]['V']  ,
            graph.node[plist[0]]['NE'] ,
            graph.node[plist[0]]['NV'] ,
            graph.node[plist[0]]['COV'],

            graph.node[plist[1]]['E']  ,
            graph.node[plist[1]]['V']  ,
            graph.node[plist[1]]['NE'] ,
            graph.node[plist[1]]['NV'] ,
            graph.node[plist[1]]['COV'],

            node_width[plist[0]]['S'],
            node_width[plist[1]]['S'],
            node_width[idx]['S']
        )
        graph.node[idx]['NE']=ne_add(*args)
        graph.node[idx]['NV']=nv_add(*args)
        graph.node[idx]['COV']=c_add(*args)

    else:
        print 'Error,',graph.node[idx]

def mult_cost(graph,idx):
    print 'mult'
    global node_width;
    plist=graph.predecessors(idx);
    if(len(plist)==2):
        args=(
            graph.node[plist[0]]['E']  ,
            graph.node[plist[0]]['V']  ,
            graph.node[plist[0]]['NE'] ,
            graph.node[plist[0]]['NV'] ,
            graph.node[plist[0]]['COV'],

            graph.node[plist[1]]['E']  ,
            graph.node[plist[1]]['V']  ,
            graph.node[plist[1]]['NE'] ,
            graph.node[plist[1]]['NV'] ,
            graph.node[plist[1]]['COV'],

            node_width[plist[0]]['S'],
            node_width[plist[1]]['S'],
            node_width[idx]['R'],
            node_width[idx]['L']
        )
        graph.node[idx]['NE']=ne_mul(*args)
        graph.node[idx]['NV']=nv_mul(*args)
        graph.node[idx]['COV']=c_mul(*args)


    else:
        print 'Error,',graph.node[idx]



def cost_func(graph, idx):
    if(graph.node[idx]['OP']=='add'):
        adder_cost(graph,idx)
    elif(graph.node[idx]['OP']=='cmul'):
        mult_cost(graph,idx)
    elif(graph.node[idx]['OP']=='data'):
        data_cost(graph,idx)
    elif(graph.node[idx]['OP']=='const'):
        data_cost(graph,idx)
    else:
        print graph.node[idx]

def bfs(graph,func):
    print 'bfs'
    queue=['0'];
    for idx in graph.nodes():
        graph.node[idx]['color']=False;
    while(len(queue)!=0):
        head=queue[0]
        queue.pop(0);
        if(graph.node[head]['color']==False):
            graph.node[head]['color']=True;
            print head,graph.node[head]
            func(graph,head)
        else:
            continue
        for idx in graph.successors(head):
            if(graph.node[idx]['color']==False):
                queue.append(idx);


def covertData(graph, idx):
    if('E' in graph.node[idx]):
        graph.node[idx]['E']=float(graph.node[idx]['E'])
        graph.node[idx]['V']=float(graph.node[idx]['V'])
        graph.node[idx]['IDX']=float(graph.node[idx]['IDX'])



def stringToFloat(graph,idx):
    bfs(graph,covertData)

def bitwidthAssign(graph, idx, l_width,r_width, s_width):
    graph.node[idx]['R']=r_width;
    graph.node[idx]['L']=l_width;
    graph.node[idx]['S']=s_width;
def print_nodes(graph,idx):
    print graph.node[idx]

global node_width
if __name__ == '__main__':
    node_width=dict();
    graph=nx.read_dot('test_graph.dot')

    tot_nodes=len(graph.node)

    r=[4]*tot_nodes;
    l=[4]*tot_nodes;
    s=[4]*tot_nodes;
    for i in range(0,tot_nodes):
        if(graph.node[str(i)]['OP']=='cmul'):
            s[i]=r[i]+l[i];
        if(graph.node[str(i)]['OP']=='const'):
            s[i]=32;
    print r
    print l
    print s

    for i in range(0,tot_nodes):
        node_width[str(i)]=dict()
        node_width[str(i)]['R']=r[i]
        node_width[str(i)]['L']=l[i]
        node_width[str(i)]['S']=s[i]

    bfs(graph,stringToFloat)
    print 'HHH'
    bfs(graph,cost_func)
    bfs(graph,print_nodes)
