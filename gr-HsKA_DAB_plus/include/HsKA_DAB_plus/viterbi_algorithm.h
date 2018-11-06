#ifndef VITERBI_ALGORITHM_H
#define VITERBI_ALGORITHM_H

#include <gnuradio/digital/metric_type.h>
#include <limits>

#define FLT_MAX std::numeric_limits<float>::max()
#define INF std::numeric_limits<float>::infinity()


// Kopie aus den GnuRadio-Source -> Einfachste Möglichkeit, den Viterbi-Algorithmus einzubinden

namespace gr
{
	namespace trellis
	{
		template <class T>
    	void calc_metric(int O, int D, const std::vector<T> &TABLE, const T *input,
		     float *metric, digital::trellis_metric_type_t type)
		{
			float minm = FLT_MAX;
			int minmi = 0;

			switch(type) 
			{
			case digital::TRELLIS_EUCLIDEAN:
				for(int o = 0; o < O; o++) 
				{
					metric[o] = 0.0;
					
					for(int m = 0; m < D; m++) 
					{
						T s = input[m]-TABLE[o*D+m];
						//gr_complex sc(1.0*s,0);
						//metric[o] += (sc*conj(sc)).real();
						metric[o] += s * s;
					}
				}
				break;
					
			case digital::TRELLIS_HARD_SYMBOL:
				for(int o = 0; o < O; o++)
				{
					metric[o] = 0.0;
					
					for(int m = 0; m < D; m++) 
					{
						T s = input[m]-TABLE[o*D+m];
						//gr_complex sc(1.0*s,0);
						//metric[o] + =(sc*conj(sc)).real();
						metric[o] += s * s;
					}
					
					if(metric[o] < minm) 
					{
						minm = metric[o];
						minmi = o;
					}
				}
					
				for(int o = 0; o < O; o++) 
				{
					metric[o] = (o == minmi ? 0.0 : 1.0);
				}
				break;
					
			case digital::TRELLIS_HARD_BIT:
				throw std::runtime_error("calc_metric: Invalid metric type (not yet implemented).");
				break;
					
			default:
				throw std::runtime_error("calc_metric: Invalid metric type.");
			}
		}

		
		template <class Ti, class To> 
		void viterbi_algorithm_combined(
			int I,
			int S,
			int O,
			const std::vector<int> &NS,
			const std::vector<int> &OS,
			const std::vector< std::vector<int> > &PS,
			const std::vector< std::vector<int> > &PI,
			int K,
			int S0,
			int SK,
			int D,
			const std::vector<Ti> &TABLE,
			gr::digital::trellis_metric_type_t TYPE,
			const Ti *in,
			To *out)
		{
			std::vector<int> trace(S*K);
			std::vector<float> alpha(S*2);
			float *metric = new float[O];
			int alphai;
			float norm,mm,minm;
			int minmi;
			int st;

			if(S0<0) 
			{ // initial state not specified
				for(int i=0;i<S;i++) alpha[0*S+i]=0;
			}
			else
			{
				for(int i=0;i<S;i++)
					alpha[0*S+i]=INF;
				alpha[0*S+S0]=0.0;
			}

			alphai=0;
			for(int k=0;k<K;k++) 
			{
				calc_metric(O, D, TABLE, &(in[k*D]), metric, TYPE); // calc metrics
				norm=INF;

				for(int j=0;j<S;j++)
				{ // for each next state do ACS
					minm=INF;
					minmi=0;
					for(unsigned int i=0;i<PS[j].size();i++) 
					{
						//int i0 = j*I+i;
						if((mm=alpha[alphai*S+PS[j][i]]+metric[OS[PS[j][i]*I+PI[j][i]]])<minm)
						{
							minm = mm;
							minmi = i;
						}
					}
					trace[k*S+j]=minmi;
					alpha[((alphai+1)%2)*S+j]=minm;

					if(minm<norm) 
						norm=minm;
				}

				for(int j=0;j<S;j++)
					alpha[((alphai+1)%2)*S+j]-=norm; // normalize total metrics so they do not explode
				alphai=(alphai+1)%2;
			}

			if(SK<0)
			{ // final state not specified
				minm=INF;
				minmi=0;

				for(int i=0;i<S;i++)
				{
					if((mm=alpha[alphai*S+i])<minm)
					{
						minm=mm;
						minmi=i;
					}

					st=minmi;
				}
			}
			else 
			{
				st=SK;
			}

			for(int k=K-1;k>=0;k--) 
			{ // traceback
				int i0=trace[k*S+st];
				out[k]= (To) PI[st][i0];
				st=PS[st][i0];
			}

			delete [] metric;
		}
	}
}

#endif